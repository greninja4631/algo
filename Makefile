# =============================
# 🏆 GAFA流 超堅牢＆自動化Makefile【bin/mainで統一】
# =============================

CC      := gcc
WARN    := -Wall -Wextra -Werror -pedantic
STD     := -std=c99 -D_POSIX_C_SOURCE=200809L
OPT     := -O2 -g

OBJ_DIR   := build
BIN_DIR   := $(OBJ_DIR)/bin
LIB_DIR   := $(OBJ_DIR)/lib
BUILDDIR  := $(OBJ_DIR)

INC := $(foreach d, $(shell find include -type d), -I$d)
CFLAGS      := $(WARN) $(STD) $(OPT) $(INC) -MMD -MP
TEST_CFLAGS := $(CFLAGS) -DTESTING

CORE_SRC  := $(shell find src -name '*.c')
CORE_OBJS := $(CORE_SRC:%.c=$(OBJ_DIR)/%.o)
CORE_DEPS := $(CORE_OBJS:.o=.d)
TEST_SRC  := $(shell find tests -type f -name 'test_*.c' ! -name 'test_util.c')
TEST_UTIL := $(wildcard tests/util/test_util.c)
TEST_OBJS := $(TEST_SRC:%.c=$(OBJ_DIR)/%.o)
TEST_DEPS := $(TEST_OBJS:.o=.d)
TEST_MAIN := tests/test_main.c

BIN      := $(BIN_DIR)/main
TEST_BIN := $(OBJ_DIR)/run_all_tests
DOCS_DIR := docs
CLEAN_DOCS := $(DOCS_DIR)

.PHONY: all build test lint docs memcheck format clean rebuild compile_commands check-alloc check-test-decl clean-all

all: build test

build: clean $(BIN)   # <<-- ビルド前に必ずクリーン！

$(BIN): $(CORE_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	@./$(TEST_BIN)

$(TEST_BIN): $(CORE_OBJS) $(TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $@ $^

DEPS := $(CORE_DEPS) $(TEST_DEPS)
-include $(DEPS)

lint:
	@cppcheck --enable=all --inconclusive --error-exitcode=1 src/ include/ tests/ ci/ scripts/ config/ db/
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

clean:
	@rm -rf $(OBJ_DIR) $(CLEAN_DOCS)

rebuild: clean all

clean-all:
	@rm -rf build/ docs/ coverage/ *.o *.a *.so *.dylib