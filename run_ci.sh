#!/bin/bash
set -xe

echo "[INFO] --- CI/CD Pipeline start ---"
echo "[INFO] Current directory: $(pwd)"
ls -l

# --- 主要ディレクトリ状態を全出力（デバッグ・CI視認性UP） ---
for d in src tests src/util src/ds src/algo include include/ds include/util; do
    echo "[INFO] $d :"
    ls -l $d || echo "[WARN] $d does not exist"
done

# --- 必須ツールのインストール（CI環境用・ローカルでも無害/既存ならskip） ---
if command -v apt-get >/dev/null 2>&1; then
     apt-get update
     apt-get install -y doxygen cppcheck clang-tidy valgrind || true
fi

# --- クリーンビルド ---
make clean

# --- コード整形（存在しなければskip, formatは任意導入） ---
make format || true

# --- 本体ビルド ---
make build

# --- clang-tidy (malloc禁止/警告fail CI厳守) ---
find src/ include/ -name "*.c" -or -name "*.h" > files.txt
clang-tidy -p . --warnings-as-errors='*' $(cat files.txt)

# --- malloc直呼び禁止（grepチェック） ---
echo "[CI] malloc/calloc/realloc/free 直呼び禁止チェック"
if grep -R -n --include="*.c" --include="*.h" '\b\(malloc\|calloc\|realloc\|free\)\b' src/ include/ | grep -v 'ds_malloc\|ds_free\|ds_calloc\|ds_realloc'; then
    echo '[ERROR] 禁止API（malloc/calloc/realloc/free）の直呼びを検出！'
    exit 1
else
    echo '[PASS] 禁止APIの直呼びは検出されませんでした。'
fi

# --- ユニットテスト ---
make test

# --- Valgrindによるメモリチェック ---
if command -v valgrind >/dev/null 2>&1; then
    make memcheck || true
else
    echo "[INFO] Valgrind not installed; skipping memcheck"
fi

# --- 静的解析（cppcheck/clang-tidy。失敗でもCI続行可） ---
make lint || true

# --- ドキュメント自動生成（Doxygen有無はプロジェクトに依存。なければ無視） ---
make docs || true

# --- 成果物・成果ディレクトリ一覧（ビルド結果もCIログに残す） ---
ls -l build || echo "[INFO] build dir not exist"
ls -l docs  || echo "[INFO] docs dir not exist"

echo "[INFO] --- CI/CD Pipeline finished ---"