#!/usr/bin/env bash
# ------------------------------------------------------------------------
# run_ci.sh  ―  ガイドライン 2025-07 改訂版 CI/CD パイプライン
# 要件:
#   * bash 4+ / set -euo pipefail
#   * ビルドは Docker 内で実行する想定（ホストでも動作は可）
#   * 終了ステータス 0 以外 → Jenkins / GitHub Actions は即 Fail
# ------------------------------------------------------------------------
set -euo pipefail
IFS=$'\n\t'
echo "[INFO] --- CI/CD Pipeline start ---"
echo "[INFO] PWD           : $(pwd)"
echo "[INFO] DATE(UTC)     : $(date -u '+%Y-%m-%dT%H:%M:%SZ')"
echo "[INFO] COMMIT        : ${CI_COMMIT_SHA:-unknown}"

# ------------------------------------------------------------------------
# 0. 主要ディレクトリ可視化（デバッグ用）
# ------------------------------------------------------------------------
for d in src tests include; do
    echo "[INFO] Tree of $d"
    [ -d "$d" ] && find "$d" -maxdepth 2 -type f | head -n 20 || echo "  (not found)"
done

# ------------------------------------------------------------------------
# 1. 環境依存ツールのインストール（ホスト実行時のみ）
#    * Docker イメージ内には事前インストール済み想定
# ------------------------------------------------------------------------
if command -v apt-get >/dev/null 2>&1; then
  sudo -n true >/dev/null 2>&1 && SUDO=sudo || SUDO=
  ${SUDO} apt-get update -qq
  ${SUDO} apt-get install -y -qq \
        build-essential clang clang-tidy cppcheck valgrind doxygen gcovr
fi

# ------------------------------------------------------------------------
# 2. クリーンビルド
# ------------------------------------------------------------------------
make clean
CFLAGS='-Wall -Wextra -Werror -pedantic -std=c11 -D_FORTIFY_SOURCE=2 -fstack-protector-strong -O2 -g'
export CFLAGS
make build

# ------------------------------------------------------------------------
# 3. フォーマット (存在すれば)
# ------------------------------------------------------------------------
make format   || echo "[INFO] Format target skipped"

# ------------------------------------------------------------------------
# 4. 静的解析 ― clang-tidy 全ファイル (警告→Fail)
# ------------------------------------------------------------------------
find src include -name '*.c' -print0 | xargs -0 -r clang-tidy \
      --warnings-as-errors='*' --quiet \
      -- -Iinclude -std=c11 $CFLAGS

# ------------------------------------------------------------------------
# 5. 禁止 API grep (malloc/calloc/realloc/free 直呼び)
# ------------------------------------------------------------------------
echo "[CI] Forbidden malloc-family 呼び出しチェック"
if grep -R --line-number --include='*.c' --include='*.h' \
      -E '\b(malloc|calloc|realloc|free)\b' src include \
      | grep -vE 'ds_(malloc|calloc|realloc|free)'; then
  echo '[ERROR] Forbidden memory API detected'
  exit 1
fi
echo '[PASS] Forbidden memory API none'

# ------------------------------------------------------------------------
# 6. cppcheck (警告→Fail)
# ------------------------------------------------------------------------
cppcheck --enable=all --error-exitcode=1 --inline-suppr src include

# ------------------------------------------------------------------------
# 7. ユニットテスト
# ------------------------------------------------------------------------
make test

# ------------------------------------------------------------------------
# 8. Valgrind（“definitely lost” = 0 を強制）
# ------------------------------------------------------------------------
if command -v valgrind >/dev/null 2>&1; then
  echo "[CI] Valgrind memcheck"
  valgrind --leak-check=full --error-exitcode=1 \
           --suppressions=valgrind.supp \
           build/tests/test_main
else
  echo "[WARN] valgrind not found; skipping memcheck"
fi

# ------------------------------------------------------------------------
# 9. カバレッジ (gcovr, optional)
# ------------------------------------------------------------------------
if command -v gcovr >/dev/null 2>&1; then
  make coverage || echo "[INFO] coverage skipped"
fi

# ------------------------------------------------------------------------
# 10. Doxygen (docs 生成は警告扱い, 生成失敗≠CI Fail)
# ------------------------------------------------------------------------
make docs || echo "[INFO] docs skipped"

# ------------------------------------------------------------------------
# 11. 成果物一覧
# ------------------------------------------------------------------------
echo "[INFO] build/ tree (top level)"
[ -d build ] && find build -maxdepth 2 | head -n 20 || echo "(empty)"
echo "[INFO] docs/ tree"
[ -d docs  ] && find docs  -maxdepth 2 | head -n 20 || echo "(empty)"

echo "[INFO] --- CI/CD Pipeline finished SUCCESS ---"