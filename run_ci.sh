#!/bin/bash
set -xe

echo "[INFO] Current directory:"
pwd
ls -l

echo "[INFO] src directory:"
ls -l src || echo "[WARN] src does not exist"
echo "[INFO] tests directory:"
ls -l tests || echo "[WARN] tests does not exist"
echo "[INFO] src/util directory:"
ls -l src/util || echo "[WARN] src/util does not exist"

# 必要なツールのインストール（CI環境のみ/ローカルは通常不要）
if command -v apt-get >/dev/null 2>&1; then
    apt-get update && apt-get install -y doxygen cppcheck clang-tidy valgrind || true
fi

# クリーン
make clean

# コード整形（あれば）
make format || true

# ビルド: src/main.c と src/*.c をリンク（main関数用実行ファイルを build/main などに出力）
make build

# ユニットテストを全て実行
make test

# Valgrindによるメモリチェック（test実行バイナリ対象）
if command -v valgrind >/dev/null 2>&1; then
    make memcheck
else
    echo "[INFO] Valgrind not installed; skipping memcheck"
fi

# 静的解析
make lint || true

# ドキュメント自動生成（Doxygen推奨）
make docs || true

# 成果物一覧表示
ls -l build || echo "[INFO] build dir not exist"
ls -l docs || echo "[INFO] docs dir not exist"