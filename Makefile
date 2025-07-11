<<<<<<< HEAD
<<<<<<< HEAD
# ================================
# Makefile 完全版
# ================================

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g -O2 -Iinclude
=======
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g -O2 -Iinclude -Iinclude/ds
SRC     := $(wildcard src/*.c) $(wildcard src/util/*.c)
>>>>>>> feature
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

<<<<<<< HEAD
.PHONY: all build test test_% stats stat_test lint docs memcheck clean
=======
# ユニットテスト（全テスト自動走査）
TESTS   := $(wildcard tests/test_*.c)
TEST_BIN := $(OBJ_DIR)/test_bin
=======
# ======================================================================
#  Makefile ― Data-Structures Project
#  ─────────────────────────────────────────────────────────────────────
#  * GCC / C99 / 全警告エラー化
#  * 本体・テスト分離ビルド
#  * logger.c は include/util/ に置いたままリンクエラーを回避
#  * tests/util/ 配下のヘッダを参照できるようインクルードパスを追加
# ======================================================================

# 0. コンパイラ & フラグ
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -pedantic -std=c99 -O2 -g \
           -D_POSIX_C_SOURCE=200809L \
           -Iinclude -Iinclude/ds -Iinclude/util -Iinclude/algo \
           -Itests/include -Itests/include/ds -Itests/include/util -Itests/include/algo \
           -Itests/util -Itests/ds -Itests/algo

# 出力ディレクトリ
OBJ_DIR  := build
DOCS_DIR := docs

# ----------------------------------------------------------------------
# 1. コアソース（ライブラリ本体）
#    - include/util/*.c を追加して logger.c を取り込む
# ----------------------------------------------------------------------
CORE_SRC := $(wildcard src/*.c)            \
            $(wildcard src/ds/*.c)         \
            $(wildcard src/util/*.c)       \
            $(wildcard include/util/*.c)   \
            $(wildcard src/algo/*.c)
>>>>>>> feature

TEST_UTIL := tests/util/test_util.c
TEST_SRC  := $(shell find tests -type f -name 'test_*.c' ! -name 'test_util.c')

SRC := $(CORE_SRC) $(TEST_UTIL) $(TEST_SRC)

<<<<<<< HEAD
.PHONY: all build stats test clean lint docs memcheck
>>>>>>> feature
=======
BIN        := $(OBJ_DIR)/main
TEST_BIN   := $(OBJ_DIR)/run_all_tests
TEST_MAIN  := tests/test_main.c
>>>>>>> feature

# ----------------------------------------------------------------------
# 5. ルール
# ----------------------------------------------------------------------
.PHONY: all build test lint docs memcheck clean format

<<<<<<< HEAD
# --- 本番用ビルド ---
build:
	@mkdir -p $(OBJ_DIR)
<<<<<<< HEAD
	$(CC) $(CFLAGS) $(INC) -o $(BIN) src/main.c $(SRC_NOMAIN)

# --- 全テスト自動実行 ---
test: build
	@for file in $(TESTS); do \
		echo "===== RUN: $$file ====="; \
		$(CC) $(CFLAGS) $(INC) -o $(TEST_BIN) $$file $(SRC_NOMAIN); \
=======
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)
=======
all: build test

# 5-1. 本体ビルド
build: $(CORE_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(BIN) $^
>>>>>>> feature

# 5-2. テストビルド & 実行
test: $(SRC) $(TEST_MAIN)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(TEST_BIN) $^
	./$(TEST_BIN)

<<<<<<< HEAD
# --- ユニットテスト ---
test: build
	@for file in $(TESTS); do \
		echo "===== RUN: $$file ====="; \
		$(CC) $(CFLAGS) -o $(TEST_BIN) $$file $(SRC); \
>>>>>>> feature
		./$(TEST_BIN); \
		rm -f $(TEST_BIN); \
	done
	@echo "All unit tests passed."

<<<<<<< HEAD
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
=======
# --- 統計API用テスト ---
stat_test:
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(STAT_TEST_BIN) $(STAT_TEST_SRC) src/statistics.c
	./$(STAT_TEST_BIN)
>>>>>>> feature

# --- 静的解析 ---
lint:
	cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/
<<<<<<< HEAD
	clang-tidy src/*.c src/ds/*.c -- -Iinclude || true
=======
	clang-tidy src/*.c src/util/*.c -- -Iinclude -Iinclude/ds || true
>>>>>>> feature
=======
# 5-3. 静的解析
lint:
	@cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/ tests/
	@clang-tidy $(SRC) -- $(CFLAGS) || true
>>>>>>> feature

# 5-4. ドキュメント生成
docs:
	@doxygen Doxyfile

<<<<<<< HEAD
# --- メモリリーク検出 ---
<<<<<<< HEAD
memcheck:
	valgrind --leak-check=full $(BIN)
=======
memcheck: test
	valgrind --leak-check=full $(OBJ_DIR)/test_bin
>>>>>>> feature

# --- クリーン ---
=======
# 5-5. Valgrind メモリチェック
memcheck: test
	valgrind --leak-check=full $(TEST_BIN)

# 5-6. コードフォーマット (clang-formatを使用)
format:
	@echo "Formatting C/H files with clang-format..."
	@find src include tests -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;
	@echo "Formatting complete."

# 5-7. クリーン
>>>>>>> feature
clean:
	@rm -rf $(OBJ_DIR) \
	         $(DOCS_DIR)/api.html $(DOCS_DIR)/api \
	         $(DOCS_DIR)/architecture.html
