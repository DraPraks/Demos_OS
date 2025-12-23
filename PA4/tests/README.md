# Tests for alfm

This directory contains a simple test script for the `alfm` program that verifies the --write and --rotate features.

Requirements
- WSL (Linux environment) on Windows
- gcc in WSL (e.g., `sudo apt install build-essential`)

Running the tests

Use WSL to run the test script inside the project directory:

```bash
cd /mnt/c/Users/prako/OneDrive/Documents/Projects/Demos_OS/PA4
chmod +x tests/run_tests.sh
./tests/run_tests.sh
```

What the tests do
- Compile `2406453530_alfm.c` to `alfm` with `gcc`.
- Test `--write`:
  - Creates a logfile and checks for the header and message formatting.
  - Appends another message and verifies it was appended.
- Test `--rotate`:
  - Creates a large file, calls rotate, checks for a `.1` backup and empty original.
  - Repeats to ensure a subsequent rotation produces `.2`.

Notes
- The tests only cover features that are implemented (`--write`, `--rotate`).
- If you add `--tail` or `--info` to `main`, extend the test script accordingly.
