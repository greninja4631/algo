# ======================================================================
#  Makefile — Data-Structures Project (2025-07 Micro-Architecture Edition)
# ======================================================================

CC      := gcc
WARN    := -Wall -Wextra -Werror -pedantic
STD     := -std=c99 -D_POSIX_C_SOURCE=200809L
OPT     := -O2 -g

OBJ_DIR   := build
BUILDDIR  := $(OBJ_DIR)

INC := \
  -Iinclude            -Iinclude/ds            -Iinclude/util            -Iinclude/algo \
  -Itests/include      -Itests/include/ds      -Itests/include/util      -Itests/include/algo

CFLAGS      := $(WARN) $(STD) $(OPT) $(INC)
TEST_CFLAGS := $(CFLAGS) -DTESTING
# TEST_CFLAGS += --coverage # カバレッジ有効時

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
CLEAN_DOCS := $(DOCS_DIR)

.PHONY: all build test lint docs memcheck format clean compile_commands check-alloc

all: build test

## 本番ビルド
build: $(CORE_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(BIN) $^

## テストビルド＋実行
test: $(CORE_SRC) $(TEST_UTIL) $(TEST_SRC) $(TEST_MAIN)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN) $^
	@./$(TEST_BIN)

## 静的解析
lint:
	@cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/ tests/
	@clang-tidy $(CORE_SRC) $(TEST_SRC) -- $(CFLAGS) || true

## ドキュメント生成
docs:
	@doxygen Doxyfile

## Valgrind
memcheck: test
	valgrind --leak-check=full $(TEST_BIN)

## clang-format
format:
	@echo "[INFO] clang-format run"
	@find src include tests -type f \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +

## コンパイルDB
compile_commands:
	@echo "[INFO] generating compile_commands.json"
	@bear --output $(BUILDDIR)/compile_commands.json -- \
	  $(MAKE) -B build

## クリーン
clean:
	@rm -rf $(OBJ_DIR) $(CLEAN_DOCS)

## ────────────────────────────────────
## すべてのAPI宣言・実装が「const ds_allocator_t* alloc」を持っているかをgrepで自動検査
## ────────────────────────────────────
check-alloc:
	@echo "[CHECK] Verifying all API signatures require 'const ds_allocator_t* alloc' ..."
	@! grep -rE --include="*.h" --include="*.c" \
	    '^[a-zA-Z_][a-zA-Z0-9_ \*]+\([^\)]*\)' include/ src/ \
	    | grep -v 'const ds_allocator_t\* alloc' \
	    | grep -v '^.*\bmain\b' \
	    && echo "[PASS] All API signatures use 'const ds_allocator_t* alloc'." \
	    || (echo "[FAIL] Found APIs missing 'const ds_allocator_t* alloc' in their signature!" && exit 1)