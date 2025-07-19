<<<<<<< HEAD
/**
 * @file    tests/util/test_metrics.c
 * @brief   util/metrics モジュールのユニットテスト
 * @note    main() は tests/test_main.c に集約される
 */

#include "util/test_metrics.h"   /* void test__metrics_basic(void); */
#include "util/metrics.h"        /* 被テスト API */
#include "util/logger.h"         /* ds_log() & DS_LOG_LEVEL_*      */

/* グローバル DI アロケータ（必要なら）  */
extern const ds_allocator_t *g_alloc;

/* ───────────────────────────── */
/* 共通アサート・マクロ            */
#define DS_TEST_ASSERT(cond, msg)                                           \
    do {                                                                    \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", (msg)); }       \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",         \
                           (msg), __FILE__, __LINE__); }                    \
    } while (0)

/* ───────────────────────────── */
/* 基本動作テスト                  */
void test__metrics_basic(void)
{
    /* 1. 全リセット (@side-effect: global) */
    ds_metrics_reset_all(g_alloc);

    /* 2. カウンタ加算 */
    ds_metrics_increment(g_alloc, "test.counter");
    ds_metrics_increment(g_alloc, "test.counter");
    int64_t v = ds_metrics_get(g_alloc, "test.counter");
    DS_TEST_ASSERT(v == 2, "test.counter == 2");

    /* 3. 別カウンタ */
    ds_metrics_increment(g_alloc, "other");
    DS_TEST_ASSERT(ds_metrics_get(g_alloc, "other") == 1, "other == 1");

    /* 4. 合計チェック（get_total API が無い場合は個別集計） */
    int64_t total =
        ds_metrics_get(g_alloc, "test.counter") +
        ds_metrics_get(g_alloc, "other");
    DS_TEST_ASSERT(total == 3, "total == 3");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__metrics_basic 完了");
}
=======
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
>>>>>>> feature
