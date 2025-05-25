CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g

# ソース・ヘッダ・バイナリ名
SRC     = miko/statistics.c miko/main.c
INC     = miko/statistics.h
BIN     = stats

TEST_SRC = test/test_main.c
TEST_BIN = test_main

.PHONY: all build test memcheck clean

all: build

# ---【本体ビルド】---
build: $(SRC)
	$(CC) $(CFLAGS) -I./miko -o $(BIN) $(SRC)

# ---【テストビルド】---
test: $(TEST_SRC) miko/statistics.c
	$(CC) $(CFLAGS) -I./miko -o $(TEST_BIN) $(TEST_SRC) miko/statistics.c

# ---【メモリリーク検査（Valgrind）】---
memcheck: test
	valgrind --leak-check=full ./$(TEST_BIN)

# ---【クリーンアップ】---
clean:
	rm -f $(BIN) $(TEST_BIN)