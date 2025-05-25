CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g

SRC     = src/statistics.c src/main.c
INC     = src/statistics.h
BIN     = stats

TEST_SRC = test/test_main.c
TEST_BIN = test_main

.PHONY: all clean test build

all: build

build: $(SRC)
	$(CC) $(CFLAGS) -I./src -o $(BIN) $(SRC)

test: $(TEST_SRC) src/statistics.c
	$(CC) $(CFLAGS) -I./src -o $(TEST_BIN) $(TEST_SRC) src/statistics.c

clean:
	rm -f $(BIN) $(TEST_BIN)

memcheck: test
	valgrind --leak-check=full ./$(TEST_BIN)