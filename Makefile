CC      := gcc
WARN    := -Wall -Wextra -Werror -pedantic
STD     := -std=c99 -D_POSIX_C_SOURCE=200809L
OPT     := -O2 -g

OBJ_DIR   := build
BUILDDIR  := $(OBJ_DIR)

INC := \
  -Iinclude -Iinclude/ds -Iinclude/util -Iinclude/algo \
  -Itests/include -Itests/include/ds -Itests/include/util -Itests/include/algo

CFLAGS      := $(WARN) $(STD) $(OPT) $(INC) -MMD -MP
TEST_CFLAGS := $(CFLAGS) -DTESTING

CORE_SRC   := $(wildcard src/*.c) $(wildcard src/ds/*.c) $(wildcard src/util/*.c) $(wildcard src/algo/*.c)
CORE_OBJS  := $(CORE_SRC:%.c=$(OBJ_DIR)/%.o)
CORE_DEPS  := $(CORE_OBJS:.o=.d)

TEST_UTIL  := tests/util/test_util.c
TEST_SRC   := $(shell find tests -type f -name 'test_*.c' ! -name 'test_util.c')
TEST_OBJS  := $(TEST_SRC:%.c=$(OBJ_DIR)/%.o)
TEST_DEPS  := $(TEST_OBJS:.o=.d)
TEST_MAIN  := tests/test_main.c

BIN        := $(OBJ_DIR)/main
TEST_BIN   := $(OBJ_DIR)/run_all_tests
DOCS_DIR   := docs
CLEAN_DOCS := $(DOCS_DIR)

.PHONY: all build test lint docs memcheck format clean rebuild compile_commands check-alloc

all: build test

build: $(BIN)

$(BIN): $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	@./$(TEST_BIN)

$(TEST_BIN): $(CORE_OBJS) $(TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $@ $^

lint:
	@cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/ tests/
	@clang-tidy $(CORE_SRC) $(TEST_SRC) -- $(CFLAGS) || true

docs:
	@doxygen Doxyfile

memcheck: test
	valgrind --leak-check=full $(TEST_BIN)

format:
	@find src include tests -type f \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +

compile_commands:
	@bear --output $(BUILDDIR)/compile_commands.json -- \
	  $(MAKE) -B build

clean:
	@rm -rf $(OBJ_DIR) $(CLEAN_DOCS)

rebuild: clean all

check-alloc:
	@echo "[CHECK] Verifying all API signatures require 'const ds_allocator_t* alloc' ..."
	@! grep -rE --include="*.h" --include="*.c" \
	    '^[a-zA-Z_][a-zA-Z0-9_ \*]+\([^\)]*\)' include/ src/ \
	    | grep -v 'const ds_allocator_t\* alloc' \
	    | grep -v '^.*\bmain\b' \
	    && echo "[PASS] All API signatures use 'const ds_allocator_t* alloc'." \
	    || (echo "[FAIL] Found APIs missing 'const ds_allocator_t* alloc' in their signature!" && exit 1)

# --- ヘッダ依存自動生成・include ---
DEPS := $(CORE_DEPS) $(TEST_DEPS)
-include $(DEPS)