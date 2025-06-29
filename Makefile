# ================================
# Makefile 完全版
# ================================

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g -O2 -Iinclude
OBJ_DIR := build
BIN     := $(OBJ_DIR)/main

# --- ソース検出 ---
SRC_MAIN   := $(wildcard src/*.c)
SRC_DS     := $(wildcard src/ds/*.c)
SRC_NOMAIN := $(filter-out src/main.c, $(SRC_MAIN)) $(SRC_DS)
INC        := -Iinclude

# --- テスト検出 ---
TESTS      := $(wildcard tests/test_*.c)
TEST_BIN   := $(OBJ_DIR)/test_bin

# --- ドキュメント・静的解析 ---
DOCS_DIR := docs

.PHONY: all build test test_% stats stat_test lint docs memcheck clean

# --- デフォルト ---
all: build

# --- 本番用ビルド ---
build:
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -o $(BIN) src/main.c $(SRC_NOMAIN)

# --- 全テスト自動実行 ---
test: build
	@for file in $(TESTS); do \
		echo "===== RUN: $$file ====="; \
		$(CC) $(CFLAGS) $(INC) -o $(TEST_BIN) $$file $(SRC_NOMAIN); \
		./$(TEST_BIN); \
		rm -f $(TEST_BIN); \
	done
	@echo "All unit tests passed."

# --- 個別テスト（例: make test_stack）---
test_%: build
	@name=$*; \
	test_file=tests/test_$${name}.c; \
	if [ -f $$test_file ]; then \
		echo "===== RUN: $$test_file ====="; \
		$(CC) $(CFLAGS) $(INC) -o $(OBJ_DIR)/test_$${name} $$test_file $(SRC_NOMAIN); \
		./$(OBJ_DIR)/test_$${name}; \
		rm -f $(OBJ_DIR)/test_$${name}; \
	else \
		echo "No such test: $$test_file"; \
	fi

# --- 統計API単体ビルド（例: stats/統計専用実行）---
stats:
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -o $(OBJ_DIR)/stats src/statistics.c

# --- 統計API用テスト ---
stat_test:
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -o $(OBJ_DIR)/test_stat tests/test_statistics.c src/statistics.c
	./$(OBJ_DIR)/test_stat

# --- 静的解析 ---
lint:
	cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/
	clang-tidy src/*.c src/ds/*.c -- -Iinclude || true

# --- ドキュメント生成 ---
docs:
	doxygen Doxyfile

# --- メモリリーク検出 ---
memcheck:
	valgrind --leak-check=full $(BIN)

# --- クリーン ---
clean:
	rm -rf $(OBJ_DIR) $(DOCS_DIR)/api.html $(DOCS_DIR)/api $(DOCS_DIR)/architecture.html
