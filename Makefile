CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g

SRC     = miko/statistics.c miko/main.c
INC     = miko/statistics.h
BIN     = stats

TEST_SRC = test/test_main.c
TEST_BIN = test_main

.PHONY: all clean test build

all: build

build: $(SRC)
	$(CC) $(CFLAGS) -I./miko -o $(BIN) $(SRC)

test: $(TEST_SRC) miko/statistics.c
	$(CC) $(CFLAGS) -I./miko -o $(TEST_BIN) $(TEST_SRC) miko/statistics.c

clean:
	rm -f $(BIN) $(TEST_BIN)