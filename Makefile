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

TEST_UTIL := tests/util/test_util.c
TEST_SRC  := $(shell find tests -type f -name 'test_*.c' ! -name 'test_util.c')

SRC := $(CORE_SRC) $(TEST_UTIL) $(TEST_SRC)

BIN        := $(OBJ_DIR)/main
TEST_BIN   := $(OBJ_DIR)/run_all_tests
TEST_MAIN  := tests/test_main.c

# ----------------------------------------------------------------------
# 5. ルール
# ----------------------------------------------------------------------
.PHONY: all build test lint docs memcheck clean format

all: build test

# 5-1. 本体ビルド
build: $(CORE_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(BIN) $^

# 5-2. テストビルド & 実行
test: $(SRC) $(TEST_MAIN)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(TEST_BIN) $^
	./$(TEST_BIN)

# 5-3. 静的解析
lint:
	@cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/ tests/
	@clang-tidy $(SRC) -- $(CFLAGS) || true

# 5-4. ドキュメント生成
docs:
	@doxygen Doxyfile

# 5-5. Valgrind メモリチェック
memcheck: test
	valgrind --leak-check=full $(TEST_BIN)

# 5-6. コードフォーマット (clang-formatを使用)
format:
	@echo "Formatting C/H files with clang-format..."
	@find src include tests -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;
	@echo "Formatting complete."

# 5-7. クリーン
clean:
	@rm -rf $(OBJ_DIR) \
	         $(DOCS_DIR)/api.html $(DOCS_DIR)/api \
	         $(DOCS_DIR)/architecture.html
