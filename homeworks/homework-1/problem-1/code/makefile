# Simple, portable Makefile (no profiling flags).
# Build: make
# Clean: make clean

CC      := cc
CFLAGS  := -O2 -std=c11 -Wall -Wextra -Iinclude
LDFLAGS :=

SRC := src/main.c src/sort.c src/io.c src/options.c
BIN := layout_sort_file_profile

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

clean:
	rm -f $(BIN)
