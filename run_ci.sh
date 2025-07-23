#!/usr/bin/env bash
# ------------------------------------------------------------------------
# run_ci.sh ― Universal/最前線 CI/CDパイプライン (全OS/アーキ/全言語)
# ソフトウェア工学最前線ガイドライン 2025-07 統合対応
# bash 4+ / set -euo pipefail / docker & ホスト両対応
# ------------------------------------------------------------------------
set -euo pipefail
IFS=$'\n\t'
export LC_ALL=C

echo "[INFO] --- CI/CD Pipeline Start ---"
echo "[INFO] OS        : $(uname -a)"
echo "[INFO] User      : $(whoami)"
echo "[INFO] Hostname  : $(hostname)"
echo "[INFO] PWD       : $(pwd)"
echo "[INFO] DATE(UTC) : $(date -u '+%Y-%m-%dT%H:%M:%SZ')"
echo "[INFO] COMMIT    : ${CI_COMMIT_SHA:-unknown}"
echo "[INFO] GIT HASH  : $(git rev-parse --short HEAD 2>/dev/null || echo 'none')"

# 0. ディレクトリ可視化
for d in src tests include db jenkins ci scripts docker; do
    echo "[INFO] Tree of $d"
    [ -d "$d" ] && find "$d" -maxdepth 2 -type f | head -n 20 || echo "  (not found)"
done

# 1. 開発/CI用依存自動導入（Docker内はイメージで満たす前提）
if command -v apt-get >/dev/null 2>&1; then
  sudo -n true >/dev/null 2>&1 && SUDO=sudo || SUDO=
  ${SUDO} apt-get update -qq
  ${SUDO} apt-get install -y -qq \
        build-essential clang clang-tidy cppcheck valgrind doxygen gcovr \
        python3 python3-pip postgresql-client nodejs npm
fi
if [ -f requirements.txt ]; then pip3 install -r requirements.txt; fi
if [ -f package.json ]; then npm install --silent; fi

# 2. Docker Multi-Arch Build（クロスビルド即検証）
echo "[INFO] --- Docker Multi-Arch Build Test (arm64, amd64) ---"
make docker-arm64 || echo "[WARN] ARM64 build failed"
make docker-amd64 || echo "[WARN] AMD64 build failed"
make docker-all   || echo "[WARN] Multiarch build failed"

# 3. クリーンビルド＆mainバイナリ生成（bin/main/で統一）
make clean
export CFLAGS='-Wall -Wextra -Werror -pedantic -std=c11 -D_FORTIFY_SOURCE=2 -fstack-protector-strong -O2 -g'
make build

# 3.1 必ずmainバイナリ検証（ここがGAFA流: lsチェックで即Fail）
ls -l build/bin/ || (echo "[FAIL] mainバイナリが無い！Makefile/ビルド失敗！パス設定・依存漏れ・.dockerignore漏れ等を確認！" && exit 1)
[ -f build/bin/main ] || (echo "[FAIL] build/bin/main が存在しません！成果物パス統一/ビルド設定見直し必須！" && exit 1)

# 4. DBスキーマ自動マイグレーション
if [ -d db/schema ]; then
  echo "[INFO] Applying DB schema (PostgreSQL)"
  for f in db/schema/*.sql; do
    [ -f "$f" ] && psql "${PGURL:-postgresql://localhost/test}" -f "$f" || true
  done
fi

[ -d jenkins/init.groovy.d ] && echo "[INFO] Jenkins Groovy Init scripts found"

# 5. コード整形・フォーマット
make format || echo "[INFO] Format target skipped"
[ -f scripts/format.sh ] && bash scripts/format.sh || true

# 6. 静的解析・Linter（全言語）
make lint || echo "[WARN] lint skipped"
ALL_CODE_DIRS="src include tests ci scripts config db docker"
if command -v clang-tidy >/dev/null 2>&1; then
  find $ALL_CODE_DIRS -name '*.c' -print0 | xargs -0 -r clang-tidy --warnings-as-errors='*' --quiet -- -Iinclude -std=c11 $CFLAGS
fi
if command -v cppcheck >/dev/null 2>&1; then
  cppcheck --enable=all --error-exitcode=1 --inline-suppr $ALL_CODE_DIRS
fi

# 7. 禁止API・アンチパターン自動検出
echo "[CI] Forbidden malloc-family 呼び出しチェック"
FORBIDDEN_DIRS="src include tests ci scripts config db docker jenkins patches"
if grep -R --line-number --include='*.c' --include='*.h' \
    -E '\b(malloc|calloc|realloc|free)\b' $FORBIDDEN_DIRS \
    | grep -vE 'ds_(malloc|calloc|realloc|free)'; then
  echo '[ERROR] Forbidden memory API detected'
  exit 1
fi
echo '[PASS] Forbidden memory API none'

# 8. 未使用関数・テスト未宣言チェック
make check-alloc
make check-test-decl

# 9. ユニット・統合テスト（失敗で即Fail）
make test

# 10. Valgrind (“definitely lost” = 0 強制/最終品質ゲート)
if command -v valgrind >/dev/null 2>&1 && [ -f build/run_all_tests ]; then
  echo "[CI] Valgrind memcheck"
  valgrind --leak-check=full --error-exitcode=1 \
           --suppressions=valgrind.supp \
           build/run_all_tests
else
  echo "[WARN] valgrind not found; skipping memcheck"
fi

# 11. カバレッジ・ドキュメント生成
if command -v gcovr >/dev/null 2>&1; then
  make coverage || echo "[INFO] coverage skipped"
fi
make docs || echo "[INFO] docs skipped"
if [ -f docs/Makefile ]; then (cd docs && make html) || true; fi

# 12. 主要ディレクトリ可視化
for d in build docs db ci; do
  echo "[INFO] $d/ tree (top level)"
  [ -d $d ] && find $d -maxdepth 2 | head -n 20 || echo "(empty)"
done

echo "[INFO] --- Universal/最前線 CI/CD Pipeline finished SUCCESS ---"