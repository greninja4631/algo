CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g

# ソース・ヘッダ・バイナリ名
SRC     = miko/statistics.c miko/main.c
INC     = miko/statistics.h
BIN     = stats

TEST_SRC = test/test_main.c
TEST_BIN = test_main

.PHONY: all clean test build

all: build

# ---【本体ビルド】---
# -I./miko でヘッダ探索パス指定
build: $(SRC)
	$(CC) $(CFLAGS) -I./miko -o $(BIN) $(SRC)

# ---【テストビルド】---
# 必ず -I./miko を忘れずに
test: $(TEST_SRC) miko/statistics.c
	$(CC) $(CFLAGS) -I./miko -o $(TEST_BIN) $(TEST_SRC) miko/statistics.c

# ---【クリーンアップ】---
clean:
	rm -f $(BIN) $(TEST_BIN)