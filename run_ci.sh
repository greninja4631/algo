#!/bin/bash
set -xe

echo "[INFO] --- CI/CD Pipeline start ---"
echo "[INFO] Current directory: $(pwd)"
ls -l

<<<<<<< HEAD
echo "[INFO] src directory:"
ls -l src || echo "[WARN] src does not exist"
echo "[INFO] src/util directory:"
ls -l src/util || echo "[WARN] src/util does not exist"
echo "[INFO] tests directory:"
ls -l tests || echo "[WARN] tests does not exist"

# --- 必要なツールのCI側追インストール（ローカルは無視してOK） ---
=======
# --- 主要ディレクトリ状態を全出力（デバッグ・CI視認性UP） ---
for d in src tests src/util src/ds src/algo include include/ds include/util; do
    echo "[INFO] $d :"
    ls -l $d || echo "[WARN] $d does not exist"
done

# --- 必須ツールのインストール（CI環境用・ローカルでも無害/既存ならskip）---
>>>>>>> feature
if command -v apt-get >/dev/null 2>&1; then
    apt-get update
    apt-get install -y doxygen cppcheck clang-tidy valgrind || true
fi

<<<<<<< HEAD
# --- クリーン ---
make clean || true

# --- コード整形（formatターゲットがあれば実行、なければスルー） ---
if grep -q "^format:" Makefile; then
    make format
=======
# --- クリーンビルド ---
make clean

# --- コード整形（存在しなければskip, formatは任意導入） ---
make format || true

# --- 本体ビルド（main関数バイナリを生成） ---
make build

# --- ユニットテスト（全API/全モジュール一括・mainは1つだけ）---
make test

# --- Valgrindによるメモリチェック（test_mainバイナリに限る）---
if command -v valgrind >/dev/null 2>&1; then
    make memcheck
>>>>>>> feature
else
    echo "[INFO] formatターゲットなし"
fi

<<<<<<< HEAD
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
=======
# --- 静的解析（cppcheck/clang-tidy。失敗でもCI続行可） ---
make lint || true

# --- ドキュメント自動生成（Doxygen有無はプロジェクトに依存。なければ無視） ---
make docs || true

# --- 成果物・成果ディレクトリ一覧（ビルド結果もCIログに残す） ---
ls -l build || echo "[INFO] build dir not exist"
ls -l docs  || echo "[INFO] docs dir not exist"

echo "[INFO] --- CI/CD Pipeline finished ---"
>>>>>>> feature
