# ======================================================================
#  Makefile — Data-Structures Project  (2025-07 Micro-Architecture Edition)
# ======================================================================

# ────────────────────────────────────
# 0. コンパイラ & 共通フラグ
# ────────────────────────────────────
CC      := gcc
WARN    := -Wall -Wextra -Werror -pedantic
STD     := -std=c99 -D_POSIX_C_SOURCE=200809L
OPT     := -O2 -g

# 生成物フォルダ（compile_commands.json の出力先にも流用）
OBJ_DIR   := build
BUILDDIR  := $(OBJ_DIR)

# インクルードパス  1 行だと長いので折り返し
INC := \
  -Iinclude            -Iinclude/ds            -Iinclude/util            -Iinclude/algo \
  -Itests/include      -Itests/include/ds      -Itests/include/util      -Itests/include/algo

CFLAGS := $(WARN) $(STD) $(OPT) $(INC)

# テスト時だけ -DTESTING を付与
TEST_CFLAGS := $(CFLAGS) -DTESTING
# 例）カバレッジを採りたければ ↓ を有効化
# TEST_CFLAGS += --coverage

# ────────────────────────────────────
# 1. ソースファイルセット
# ────────────────────────────────────
CORE_SRC := $(wildcard src/*.c) \
            $(wildcard src/ds/*.c) \
            $(wildcard src/util/*.c) \
            $(wildcard src/algo/*.c)

TEST_UTIL := tests/util/test_util.c
TEST_SRC  := $(shell find tests -type f -name 'test_*.c' ! -name 'test_util.c')
TEST_MAIN := tests/test_main.c

BIN      := $(OBJ_DIR)/main
TEST_BIN := $(OBJ_DIR)/run_all_tests
DOCS_DIR := docs
CLEAN_DOCS := $(DOCS_DIR)          # Doxygen 出力を丸ごと削除

# ────────────────────────────────────
# 2. ルール
# ────────────────────────────────────
.PHONY: all build test lint docs memcheck format clean compile_commands

all: build test

## 2-1 本番ビルド
build: $(CORE_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(BIN) $^

## 2-2 テストビルド＋実行
test: $(CORE_SRC) $(TEST_UTIL) $(TEST_SRC) $(TEST_MAIN)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN) $^
	@./$(TEST_BIN)

## 2-3 静的解析
lint:
	@cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/ tests/
	@clang-tidy $(CORE_SRC) $(TEST_SRC) -- $(CFLAGS) || true

## 2-4 ドキュメント生成
docs:
	@doxygen Doxyfile

## 2-5 Valgrind
memcheck: test
	valgrind --leak-check=full $(TEST_BIN)

## 2-6 clang-format
format:
	@echo "[INFO] clang-format run"
	@find src include tests -type f \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +

## 2-7 コンパイル DB（clangd / VSCode 用）
compile_commands:
	@echo "[INFO] generating compile_commands.json"
	@bear --output $(BUILDDIR)/compile_commands.json -- \
	  $(MAKE) -B build

## 2-8 クリーン
clean:
	@rm -rf $(OBJ_DIR) $(CLEAN_DOCS)