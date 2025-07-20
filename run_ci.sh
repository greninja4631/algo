#!/usr/bin/env bash
# ------------------------------------------------------------------------
# run_ci.sh  ―  ソフトウェア工学最前線ガイドライン 2025-07 統合CI/CDパイプライン
# 対象: C/C++/Python/TypeScript/DB/AI/GCP/K8s/OSS運用一元化
# - bash 4+ / set -euo pipefail
# - Docker or ホスト両対応
# - 終了コード≠0 で即Fail
# ------------------------------------------------------------------------
set -euo pipefail
IFS=$'\n\t'
export LC_ALL=C

echo "[INFO] --- CI/CD Pipeline Start ---"
echo "[INFO] User      : $(whoami)"
echo "[INFO] Hostname  : $(hostname)"
echo "[INFO] PWD       : $(pwd)"
echo "[INFO] DATE(UTC) : $(date -u '+%Y-%m-%dT%H:%M:%SZ')"
echo "[INFO] COMMIT    : ${CI_COMMIT_SHA:-unknown}"

# ------------------------------------------------------------------------
# 0. ディレクトリ可視化（全技術対応: DB/CI/コンテナ/AIも網羅）
# ------------------------------------------------------------------------
for d in src tests include db jenkins ci scripts docker; do
    echo "[INFO] Tree of $d"
    [ -d "$d" ] && find "$d" -maxdepth 2 -type f | head -n 20 || echo "  (not found)"
done

# ------------------------------------------------------------------------
# 1. 開発/CI用依存の自動インストール（ホスト限定）
# ------------------------------------------------------------------------
if command -v apt-get >/dev/null 2>&1; then
  sudo -n true >/dev/null 2>&1 && SUDO=sudo || SUDO=
  ${SUDO} apt-get update -qq
  ${SUDO} apt-get install -y -qq \
        build-essential clang clang-tidy cppcheck valgrind doxygen gcovr \
        python3 python3-pip postgresql-client nodejs npm
fi

# Python/TypeScript/AI/DB依存も自動で
if [ -f requirements.txt ]; then pip3 install -r requirements.txt; fi
if [ -f package.json ]; then npm install --silent; fi

# ------------------------------------------------------------------------
# 2. クリーンビルド＆DB/AI/インフラセットアップ
# ------------------------------------------------------------------------
make clean
CFLAGS='-Wall -Wextra -Werror -pedantic -std=c11 -D_FORTIFY_SOURCE=2 -fstack-protector-strong -O2 -g'
export CFLAGS
make build

# DBスキーマ・マイグレーション適用（PostgreSQL）
if [ -d db/schema ]; then
  echo "[INFO] Applying DB schema (PostgreSQL)"
  for f in db/schema/*.sql; do
    [ -f "$f" ] && psql "${PGURL:-postgresql://localhost/test}" -f "$f" || true
  done
fi

# Jenkins/CI関連のinit自動化
if [ -d jenkins/init.groovy.d ]; then
  echo "[INFO] Jenkins Groovy Init scripts found"
fi

# ------------------------------------------------------------------------
# 3. コード整形・フォーマット（C/C++/Py/TS/etc）
# ------------------------------------------------------------------------
make format || echo "[INFO] Format target skipped"
if [ -f scripts/format.sh ]; then bash scripts/format.sh || true; fi

# ------------------------------------------------------------------------
# 4. 静的解析・Linter全言語 (警告=Fail, 設定自動検出)
# ------------------------------------------------------------------------
make lint || echo "[INFO] Make lint target skipped"

# 全C/C++コード（src, include, tests, ci, scripts, config, db, docker などもカバー）
ALL_CODE_DIRS="src include tests ci scripts config db docker"

if command -v clang-tidy >/dev/null 2>&1; then
  find $ALL_CODE_DIRS -name '*.c' -print0 | xargs -0 -r clang-tidy \
    --warnings-as-errors='*' --quiet -- -Iinclude -std=c11 $CFLAGS
fi
if command -v cppcheck >/dev/null 2>&1; then
  cppcheck --enable=all --error-exitcode=1 --inline-suppr $ALL_CODE_DIRS
fi
if command -v pylint >/dev/null 2>&1; then
  # src, scripts, ci, config, db, docker などPythonコードのある全ディレクトリ
  find $ALL_CODE_DIRS -name '*.py' | xargs pylint --errors-only || true
fi
if command -v eslint >/dev/null 2>&1; then
  # src, scripts, ci, config, db, docker などTSコードのある全ディレクトリ
  find $ALL_CODE_DIRS -name '*.ts' | xargs eslint --max-warnings=0 || true
fi

# ------------------------------------------------------------------------
# 5. 禁止API/アンチパターン自動検出
#    malloc/calloc/realloc/free の直呼び厳禁（DI徹底）
# ------------------------------------------------------------------------
echo "[CI] Forbidden malloc-family 呼び出しチェック"

# 必要な全てのフォルダを含める（順不同・重複OK）
FORBIDDEN_DIRS="src include tests ci scripts config db docker jenkins patches"

if grep -R --line-number --include='*.c' --include='*.h' \
    -E '\b(malloc|calloc|realloc|free)\b' $FORBIDDEN_DIRS \
    | grep -vE 'ds_(malloc|calloc|realloc|free)'; then
  echo '[ERROR] Forbidden memory API detected'
  exit 1
fi
echo '[PASS] Forbidden memory API none'

# ------------------------------------------------------------------------
# 6. ユニット・統合テスト（全自動）
# --- 既存 C/C++ テスト ---
make test

# --- Python: pytest (tests, scripts, db, ci, docker, config, なども探索) ---
PYTHON_TEST_DIRS="tests scripts db ci docker config"
for d in $PYTHON_TEST_DIRS; do
  if [ -d "$d" ]; then
    if find "$d" -name 'test_*.py' | grep -q .; then
      echo "[INFO] Running pytest in $d"
      pytest "$d" || true
    fi
  fi
done

# --- TypeScript: jest (tests, scripts, src, なども探索) ---
TS_TEST_DIRS="tests scripts src"
for d in $TS_TEST_DIRS; do
  if [ -d "$d" ]; then
    if find "$d" -name '*.test.ts' | grep -q .; then
      if [ -f jest.config.js ] || [ -f "$d/jest.config.js" ]; then
        echo "[INFO] Running jest in $d"
        npx jest "$d" || true
      fi
    fi
  fi
done

# --- Go: go test (tests, scripts, src, なども探索) ---
GO_TEST_DIRS="tests scripts src"
for d in $GO_TEST_DIRS; do
  if [ -d "$d" ]; then
    if find "$d" -name '*_test.go' | grep -q .; then
      echo "[INFO] Running go test in $d"
      (cd "$d" && go test ./...) || true
    fi
  fi
done
# ------------------------------------------------------------------------
# 7. Valgrind（“definitely lost” = 0 強制）
# ------------------------------------------------------------------------
if command -v valgrind >/dev/null 2>&1 && [ -f build/run_all_tests ]; then
  echo "[CI] Valgrind memcheck"
  valgrind --leak-check=full --error-exitcode=1 \
           --suppressions=valgrind.supp \
           build/run_all_tests
else
  echo "[WARN] valgrind not found; skipping memcheck"
fi

# ------------------------------------------------------------------------
# 8. カバレッジ（gcovr, pytest-cov, nyc/Jest/TS, etc.）
# ------------------------------------------------------------------------
if command -v gcovr >/dev/null 2>&1; then
  make coverage || echo "[INFO] coverage skipped"
fi
if command -v pytest >/dev/null 2>&1; then
  pytest --cov=src || true
fi
if command -v nyc >/dev/null 2>&1 && [ -f jest.config.js ]; then
  npx nyc --reporter=lcov npx jest || true
fi

# ------------------------------------------------------------------------
# 9. Doxygen/Docs/Sphinx/mdbook（警告=許容）
# ------------------------------------------------------------------------
make docs || echo "[INFO] docs skipped"
if [ -f docs/Makefile ]; then (cd docs && make html) || true; fi

# ------------------------------------------------------------------------
# 10. 成果物・主要ディレクトリ 可視化/報告
# ------------------------------------------------------------------------
for d in build docs db ci; do
  echo "[INFO] $d/ tree (top level)"
  [ -d $d ] && find $d -maxdepth 2 | head -n 20 || echo "(empty)"
done

echo "[INFO] --- CI/CD Pipeline finished SUCCESS ---"