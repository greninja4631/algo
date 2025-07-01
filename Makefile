CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g -O2 -Iinclude -Iinclude/ds
SRC     := $(wildcard src/*.c) $(wildcard src/util/*.c)
OBJ_DIR := build
BIN     := $(OBJ_DIR)/main
DOCS_DIR := docs

# ユニットテスト（全テスト自動走査）
TESTS   := $(wildcard tests/test_*.c)
TEST_BIN := $(OBJ_DIR)/test_bin

# 統計API用
STAT_SRC := src/statistics.c src/main.c
STAT_BIN := $(OBJ_DIR)/stats

STAT_TEST_SRC := test/test_main.c
STAT_TEST_BIN := $(OBJ_DIR)/test_main

.PHONY: all build stats test clean lint docs memcheck

# --- デフォルト ---
all: build

# --- ビルド(main) ---
build: $(SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

# --- 統計APIのみビルド ---
stats: $(STAT_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(STAT_BIN) $(STAT_SRC)

# --- ユニットテスト ---
test: build
	@for file in $(TESTS); do \
		echo "===== RUN: $$file ====="; \
		$(CC) $(CFLAGS) -o $(TEST_BIN) $$file $(SRC); \
		./$(TEST_BIN); \
		rm -f $(TEST_BIN); \
	done
	@echo "All unit tests passed."

# --- 統計API用テスト ---
stat_test:
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(STAT_TEST_BIN) $(STAT_TEST_SRC) src/statistics.c
	./$(STAT_TEST_BIN)

# --- 静的解析 ---
lint:
	cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/
	clang-tidy src/*.c src/util/*.c -- -Iinclude -Iinclude/ds || true

# --- ドキュメント生成 ---
docs:
	doxygen Doxyfile

# --- メモリリーク検出 ---
memcheck: test
	valgrind --leak-check=full $(OBJ_DIR)/test_bin

# --- クリーンアップ ---
clean:
	rm -rf $(OBJ_DIR) $(DOCS_DIR)/api.html $(DOCS_DIR)/api $(DOCS_DIR)/architecture.html
