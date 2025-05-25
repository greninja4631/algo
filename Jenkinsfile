#!/bin/bash
set -xe

echo "[INFO] カレントディレクトリ:"
pwd || true
ls -l || true

echo "[INFO] srcディレクトリの中身:"
ls -l src || true

echo "[INFO] testディレクトリの中身:"
if [ -d test ]; then
    ls -l test || true
else
    echo "[WARN] test ディレクトリが存在しません"
fi

echo "[INFO] practiceディレクトリの中身:"
if [ -d practice ]; then
    ls -l practice || true
else
    echo "[WARN] practice ディレクトリが存在しません"
fi

# ルート直下Makefile（src/用）でビルド
if make clean; then
    echo "[INFO] Clean完了"
else
    echo "[WARN] make clean失敗（スルー）"
fi

# コード整形
if make -q format 2>/dev/null; then
    make format
else
    echo "[INFO] formatターゲットなし"
fi

# ビルド
if ! make build; then
    echo "[ERROR] make build失敗"
    exit 1
fi

# テスト
if ! make test; then
    echo "[ERROR] make test失敗"
    exit 1
fi

# memcheck
if command -v valgrind >/dev/null 2>&1; then
    if make -q memcheck 2>/dev/null; then
        make memcheck
    else
        echo "[INFO] memcheckターゲットなし"
    fi
else
    echo "[INFO] Valgrindがインストールされていません。memcheckスキップ"
fi

# --- 🔻 practice/ ディレクトリ用の追加ビルド/テスト ---
if [ -d practice ]; then
    echo "[INFO] practiceディレクトリで写経コードをビルド＆テスト"
    for srcfile in practice/*.c; do
        [ -e "$srcfile" ] || continue
        binfile="${srcfile%.c}.out"
        gcc -Wall -Wextra -Werror -g "$srcfile" -o "$binfile" && echo "[SUCCESS] $srcfile → $binfile"
        if [ -f "$binfile" ]; then
            echo "[INFO] $binfile 実行:"
            ./"$binfile"
        fi
    done
else
    echo "[WARN] practiceディレクトリが存在しません（スキップ）"
fi

echo "[INFO] ビルド後の成果物:"
ls -l || true

exit 0