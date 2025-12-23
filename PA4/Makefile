CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = alfm
SRC = 2406453530_alfm.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
	[ -d tests/tmp ] && rm -rf tests/tmp/* || true
