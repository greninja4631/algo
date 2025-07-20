# =============================
# 🏆 GAFA流 超堅牢＆自動化Makefile
# =============================

CC      := gcc
WARN    := -Wall -Wextra -Werror -pedantic
STD     := -std=c99 -D_POSIX_C_SOURCE=200809L
OPT     := -O2 -g

# --- ビルド/出力/カバレッジ一元管理 ---
OBJ_DIR   := build
BUILDDIR  := $(OBJ_DIR)

# --- include以下の全ディレクトリを自動で-Iパス化 ---
INC := $(foreach d, $(shell find include -type d), -I$d)

CFLAGS      := $(WARN) $(STD) $(OPT) $(INC) -MMD -MP
TEST_CFLAGS := $(CFLAGS) -DTESTING

# --- src, tests 配下の .cを全自動検出（追加に強い） ---
CORE_SRC := $(shell find src -name '*.c')
CORE_OBJS := $(CORE_SRC:%.c=$(OBJ_DIR)/%.o)
CORE_DEPS := $(CORE_OBJS:.o=.d)

# --- テスト/ユーティリティ .cも再帰取得 ---
TEST_SRC := $(shell find tests -type f -name 'test_*.c' ! -name 'test_util.c')
TEST_UTIL := $(wildcard tests/util/test_util.c)
TEST_OBJS := $(TEST_SRC:%.c=$(OBJ_DIR)/%.o)
TEST_DEPS := $(TEST_OBJS:.o=.d)
TEST_MAIN := tests/test_main.c

# --- バイナリ・成果物名 ---
BIN        := $(OBJ_DIR)/main
TEST_BIN   := $(OBJ_DIR)/run_all_tests
DOCS_DIR   := docs
CLEAN_DOCS := $(DOCS_DIR)

.PHONY: all build test lint docs memcheck format clean rebuild compile_commands check-alloc clean-all

# ====== メインターゲット ======
all: build test

build: $(BIN)

$(BIN): $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# --- build/配下に.o/.dをパターン生成 ---
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --- テストバイナリ&実行（ユニットテスト自動化） ---
test: $(TEST_BIN)
	@./$(TEST_BIN)

$(TEST_BIN): $(CORE_OBJS) $(TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $@ $^

# ====== 静的解析・Lint・Format・カバレッジ ======
lint:
	@cppcheck --enable=all --inconclusive --error-exitcode=1 \
	  src/ include/ tests/ ci/ scripts/ config/ db/
	@clang-tidy $(shell find src include tests ci scripts config db -name '*.c' -o -name '*.cpp') -- $(CFLAGS) || true

docs:
	@doxygen Doxyfile

memcheck: test
	valgrind --leak-check=full $(TEST_BIN)

format:
	@find src include tests -type f \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +

compile_commands:
	@bear --output $(BUILDDIR)/compile_commands.json -- \
	  $(MAKE) -B build

# ====== クリーン・再ビルド・全削除 ======
clean:
	@rm -rf $(OBJ_DIR) $(CLEAN_DOCS)

rebuild: clean all

clean-all:
	@rm -rf build/ docs/ coverage/ *.o *.a *.so *.dylib

# ====== API契約チェック（CI・品質保証用） ======
check-alloc:
	@echo "[CHECK] Verifying all API signatures require 'const ds_allocator_t* alloc' ..."
	@! grep -rE --include="*.h" --include="*.c" \
	    '^[a-zA-Z_][a-zA-Z0-9_ \*]+\([^\)]*\)' \
	    include/ src/ tests/ ci/ db/ scripts/ config/ \
	    | grep -v 'const ds_allocator_t\* alloc' \
	    | grep -v '^.*\bmain\b' \
	    && echo "[PASS] All API signatures use 'const ds_allocator_t* alloc'." \
	    || (echo "[FAIL] Found APIs missing 'const ds_allocator_t* alloc' in their signature!" && exit 1)
# ====== ヘッダ依存自動生成 ======
DEPS := $(CORE_DEPS) $(TEST_DEPS)
-include $(DEPS)

# ====== 拡張アドバイス ======
# - scripts/ 配下にテスト/カバレッジ/Fuzzing用shやpyが増えた場合もここで一元化管理
# - db/ config/ ci/ も将来 *.c, *.h, *.py, *.sh等が増えた時に容易にLint/Format/テスト可能
# - .envやDockerfile等は専用Makefileサブターゲット or run_ci.shに委譲推奨
# - 自動テスト・静的解析・リリースフローはrun_ci.sh等に集約しやすい設計