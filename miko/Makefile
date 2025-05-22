# =========================
# C言語プロジェクト用 Makefile
# (miko/ 配下で使用を想定)
# =========================

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g

SRC     = statistics.c
BIN     = stats

# ユニットテスト用
TEST_SRC = test/test_main.c
TEST_BIN = test_main

.PHONY: all build test memcheck clean

# デフォルトターゲット
all: build

# =========================
# 通常ビルド（miko/配下想定）
# =========================
build:
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

# =========================
# ユニットテスト
# - test/ ディレクトリ下にテストがあるか確認
# - なければ警告＆statsバイナリを直接実行（手動テスト用）
# =========================
test: build
	@echo "[TEST] ユニットテスト実行"
ifneq ("$(wildcard $(TEST_SRC))","")
	$(CC) $(CFLAGS) -o $(TEST_BIN) $(TEST_SRC) $(SRC)
	./$(TEST_BIN)
else
	@echo "⚠️  テストコード ($(TEST_SRC)) が見つかりません。statsバイナリを直接実行します。"
	@./$(BIN)
endif

# =========================
# メモリリークチェック（Valgrind）
# =========================
memcheck: build
	@echo "[MEMCHECK] Valgrindでメモリリーク検査"
	valgrind --leak-check=full ./$(BIN) 2>&1 | tee memcheck.log

# =========================
# お掃除
# =========================
clean:
	rm -f $(BIN) $(TEST_BIN) *.log

# =========================
# Docker/Jenkinsでの利用例
# =========================
# docker run -v $(pwd):/app -w /app/miko <image> make test