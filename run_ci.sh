#!/bin/bash
set -xe

echo "[INFO] Current directory:"
pwd
ls -l

echo "[INFO] src directory:"
ls -l src || echo "[WARN] src does not exist"
echo "[INFO] src/util directory:"
ls -l src/util || echo "[WARN] src/util does not exist"
echo "[INFO] tests directory:"
ls -l tests || echo "[WARN] tests does not exist"

# --- 必要なツールのCI側追インストール（ローカルは無視してOK） ---
if command -v apt-get >/dev/null 2>&1; then
    apt-get update && apt-get install -y doxygen cppcheck clang-tidy valgrind || true
fi

# --- クリーン ---
make clean || true

# --- コード整形（formatターゲットがあれば実行、なければスルー） ---
if grep -q "^format:" Makefile; then
    make format
else
    echo "[INFO] formatターゲットなし"
fi

# --- ビルド ---
make build

# --- ユニットテスト ---
make test

# --- Valgrindメモリチェック ---
if command -v valgrind >/dev/null 2>&1 && grep -q "^memcheck:" Makefile; then
    make memcheck
else
    echo "[INFO] Valgrind未導入かmemcheckターゲットなしでスキップ"
fi

# --- 静的解析 ---
make lint || true

# --- Doxygenドキュメント生成 ---
make docs || true

# --- build成果物表示 ---
ls -l build || echo "[INFO] build dir not exist"

# --- docs成果物表示 ---
ls -l docs || echo "[INFO] docs dir not exist"

echo "[INFO] CIスクリプト全処理完了"
exit 0