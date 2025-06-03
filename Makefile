# ================================
# Makefile (統計API + 多層構造 生合成)
# ================================

# --- 変数定義 ---
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g -O2 -Iinclude
SRC     := $(wildcard src/*.c) $(wildcard src/util/*.c)
INC     := -Iinclude
OBJ_DIR := build
BIN     := $(OBJ_DIR)/main
DOCS_DIR := docs
//面談
# ユニットテスト（全テスト自動走査）
TESTS   := $(wildcard tests/test_*.c)
TEST_BIN := $(OBJ_DIR)/test_bin

# 統計API用（個別ビルドにも対応：互換性維持）
STAT_SRC := src/statistics.c src/main.c
STAT_INC := src/statistics.h
STAT_BIN := $(OBJ_DIR)/stats

STAT_TEST_SRC := test/test_main.c
STAT_TEST_BIN := $(OBJ_DIR)/test_main

.PHONY: all build stats test clean lint docs memcheck

# --- デフォルト ---
all: build

# --- ビルド(main) ---
build: $(SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -o $(BIN) $(SRC)

# --- 統計APIのみビルド（既存流儀との生合成）---
stats: $(STAT_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Isrc -o $(STAT_BIN) $(STAT_SRC)

# --- ユニットテスト：tests/test_*.cを全て自動実行 ---
test: build
	@for file in $(TESTS); do \
		echo "===== RUN: $$file ====="; \
		$(CC) $(CFLAGS) $(INC) -o $(TEST_BIN) $$file $(SRC); \
		./$(TEST_BIN); \
		rm -f $(TEST_BIN); \
	done
	@echo "All unit tests passed."

# --- 統計API用テスト（互換性のため残す）---
stat_test:
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Isrc -o $(STAT_TEST_BIN) $(STAT_TEST_SRC) src/statistics.c
	./$(STAT_TEST_BIN)

# --- 静的解析（現場CI/CD・品質担保） ---
lint:
	cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/
	clang-tidy src/*.c src/util/*.c -- -Iinclude || true

# --- ドキュメント生成（Doxygen想定） ---
docs:
	doxygen Doxyfile

# --- メモリリーク検出（valgrind） ---
memcheck: test
	valgrind --leak-check=full $(OBJ_DIR)/test_bin

# --- クリーンアップ ---
clean:
	rm -rf $(OBJ_DIR) $(DOCS_DIR)/api.html $(DOCS_DIR)/api $(DOCS_DIR)/architecture.html