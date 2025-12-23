#!/usr/bin/env bash
set -euo pipefail

# Test script for alfm (run in WSL)
# Tests for --write and --rotate features only

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR"

GCC=gcc
ALFM_BIN="$ROOT_DIR/alfm"
SRC_FILE="$ROOT_DIR/2406453530_alfm.c"
TEST_DIR="$ROOT_DIR/tests/tmp"
mkdir -p "$TEST_DIR"
rm -rf "$TEST_DIR"/*

# Compile the program
echo "Compiling..."
$GCC -Wall -Wextra -std=c11 -o "$ALFM_BIN" "$SRC_FILE" || { echo "Compilation failed"; exit 1; }

# Helper functions
pass_count=0
fail_count=0

function assert_file_exists {
    if [ -f "$1" ]; then
        echo "[PASS] File exists: $1"
        pass_count=$((pass_count+1))
    else
        echo "[FAIL] File does not exist: $1"
        fail_count=$((fail_count+1))
    fi
}

function assert_nonempty {
    if [ -s "$1" ]; then
        echo "[PASS] File non-empty: $1"
        pass_count=$((pass_count+1))
    else
        echo "[FAIL] File empty: $1"
        fail_count=$((fail_count+1))
    fi
}

function assert_match {
    local file=$1
    local regex=$2
    if grep -Pq "$regex" "$file"; then
        echo "[PASS] $file matches $regex"
        pass_count=$((pass_count+1))
    else
        echo "[FAIL] $file does not match $regex"
        fail_count=$((fail_count+1))
    fi
}

function assert_output_equals {
    local out="$1"
    local expected="$2"
    if [ "$out" = "$expected" ]; then
        echo "[PASS] Output equals expected"
        pass_count=$((pass_count+1))
    else
        echo "[FAIL] Output did not equal expected. Got:\n$out\nExpected:\n$expected"
        fail_count=$((fail_count+1))
    fi
}

function assert_output_matches {
    local out="$1"
    local regex="$2"
    if echo "$out" | grep -Pq "$regex"; then
        echo "[PASS] Output matches $regex"
        pass_count=$((pass_count+1))
    else
        echo "[FAIL] Output does not match $regex. Output:\n$out"
        fail_count=$((fail_count+1))
    fi
}

# Test 1: --write creates new logfile and writes header and message
TEST_LOG="$TEST_DIR/2406453530_testfile.log"
if [ -f "$TEST_LOG" ]; then rm -f "$TEST_LOG"; fi

./alfm --write "$TEST_LOG" "Hello World"

assert_file_exists "$TEST_LOG"
assert_nonempty "$TEST_LOG"
# Check header line
assert_match "$TEST_LOG" "^========= 2406453530_Log \d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} ========"$
# Check message format
assert_match "$TEST_LOG" "^\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\]_2406453530_Log Hello World$"

# Append another message and check there are now two message lines
./alfm --write "$TEST_LOG" "Second Message"

# Count message lines (exclude header)
message_lines=$(tail -n +2 "$TEST_LOG" | wc -l)
if [ "$message_lines" -ge 2 ]; then
    echo "[PASS] Message appended successfully, message_lines=$message_lines"
    pass_count=$((pass_count+1))
else
    echo "[FAIL] Message was not appended correctly, message_lines=$message_lines"
    fail_count=$((fail_count+1))
fi

# Test 2: --rotate: create a file larger than max_size and rotate
ROT_LOG="$TEST_DIR/2406453530_rottest.log"
# Create a file larger than 1024 bytes
perl -e 'print "A" x 1500' > "$ROT_LOG"
size_before=$(stat -c%s "$ROT_LOG")
if [ "$size_before" -le 1200 ]; then
    echo "[FAIL] Failed to create large test file"
    fail_count=$((fail_count+1))
else
    echo "[INFO] Created rotate test file with size $size_before"
    pass_count=$((pass_count+1))
fi

# Ensure no previous rotated files exist
rm -f "$ROT_LOG".*

# Rotate with max_size 1024
./alfm --rotate "$ROT_LOG" 1024

# Check that first backup exists and original exists and is empty
assert_file_exists "$ROT_LOG.1"
assert_file_exists "$ROT_LOG"
size_after=$(stat -c%s "$ROT_LOG")
if [ "$size_after" -eq 0 ]; then
    echo "[PASS] Original log truncated/created empty after rotation"
    pass_count=$((pass_count+1))
else
    echo "[FAIL] Original log not empty after rotation (size=$size_after)"
    fail_count=$((fail_count+1))
fi

# Create schedule: create - rotate when .1 exists to get .2
# Create a new content and then call rotate twice
perl -e 'print "B" x 1500' > "$ROT_LOG"
# At this point .1 should already exist from the first rotation, so we can reuse it to test creating a new numbered backup (.2 or higher)

# Now rotate again (should create .2)
./alfm --rotate "$ROT_LOG" 1024

# Check .2 exists
# We'll scan for the highest numbered file
highest=0
for f in "$ROT_LOG".*; do
    if [[ -f "$f" ]]; then
        num=$(echo "$f" | sed -E 's/.*\.([0-9]+)$/\1/')
        if [[ "$num" =~ ^[0-9]+$ ]]; then
            if [ "$num" -gt "$highest" ]; then highest=$num; fi
        fi
    fi
done
if [ "$highest" -ge 2 ]; then
    echo "[PASS] Subsequent rotation created .${highest}"
    pass_count=$((pass_count+1))
else
    echo "[FAIL] Subsequent rotation did not create numbered backup properly (highest=$highest)"
    fail_count=$((fail_count+1))
fi

# Test 2b: --rotate fallback to default when size omitted
rm -f "$ROT_LOG".*
perl -e 'print "C" x 1500' > "$ROT_LOG"
./alfm --rotate "$ROT_LOG"
assert_file_exists "$ROT_LOG.1"
# Check original empty
size_after=$(stat -c%s "$ROT_LOG")
if [ "$size_after" -eq 0 ]; then
    echo "[PASS] Default rotate created empty original log"
    pass_count=$((pass_count+1))
else
    echo "[FAIL] Default rotate did not create empty original log (size=$size_after)"
    fail_count=$((fail_count+1))
fi

# Test 2c: --rotate should NOT rotate if file size == max_size
rm -f "$ROT_LOG".*
perl -e 'print "D" x 1024' > "$ROT_LOG"
# rotate with exact size: should not rotate
./alfm --rotate "$ROT_LOG" 1024
if [ -f "$ROT_LOG.1" ]; then
    echo "[FAIL] File rotated when size == max_size"
    fail_count=$((fail_count+1))
else
    echo "[PASS] No rotation performed when size == max_size"
    pass_count=$((pass_count+1))
fi

# Test 3: --tail: create a file with 10 lines and verify last N are printed
TAIL_LOG="$TEST_DIR/2406453530_tailtest.log"
rm -f "$TAIL_LOG"
for i in {1..10}; do echo "Line $i" >> "$TAIL_LOG"; done
out=$(./alfm --tail "$TAIL_LOG" 3)
expected=$(printf "Line %s\n" 8 9 10)
assert_output_equals "$out" "$expected"

# tail with N greater than number of lines should print the entire file
out=$(./alfm --tail "$TAIL_LOG" 20)
expected=$(printf "Line %s\n" {1..10})
assert_output_equals "$out" "$expected"

# Test 4: --info outputs expected fields and correct size
INFO_LOG="$TEST_DIR/2406453530_infotest.log"
rm -f "$INFO_LOG"
echo "abcd" > "$INFO_LOG"
size_expected=$(stat -c%s "$INFO_LOG")
out=$(./alfm --info "$INFO_LOG")
assert_output_matches "$out" "^File: .*2406453530_infotest.log"
assert_output_matches "$out" "^Size: $size_expected bytes$"
assert_output_matches "$out" "^Last Modified: \d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$"
assert_output_matches "$out" "^Owner UID: \d+$"
assert_output_matches "$out" "^Permission: [r-][w-][x-][r-][w-][x-][r-][w-][x-]$"

# Test 4b: --info on missing file should print an error
missing="$TEST_DIR/doesnotexist.log"
if ./alfm --info "$missing" 2>&1 | grep -q "Failed to get file status"; then
    echo "[PASS] --info prints error when file does not exist"
    pass_count=$((pass_count+1))
else
    echo "[FAIL] --info did not print expected error for missing file"
    fail_count=$((fail_count+1))
fi
# Summary

echo "\nTest summary: passes=$pass_count fails=$fail_count"
if [ "$fail_count" -ne 0 ]; then
    exit 1
fi
exit 0
