/**
 * @file   tests/util/test_metrics.c
 * @brief  util/metrics モジュールの単体テスト実装
 *
 * 2025-07 ガイドライン準拠：
 *   • 実装は *.c のみ。対応ヘッダは宣言だけ  
 *   • Include-Guard 名は DS_TEST_UTIL_METRICS_H
 */

#include "util/test_metrics.h"   /* 宣言だけ置いた専用ヘッダ */
#include "util/metrics.h"        /* テスト対象 API          */
#include "util/logger.h"

#include <stdio.h>

/* ───────── 共通アサーション ───────── */
#define DS_TEST_ASSERT(cond, msg)                                              \
    do {                                                                       \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", (msg)); }          \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",            \
                           (msg), __FILE__, __LINE__);                         \
        }                                                                      \
    } while (0)

/* ──────────────────────────────────────────
 * 基本動作テスト
 * ────────────────────────────────────────── */
void test__metrics_basic(void)
{
    /* 1. 初期化／リセット */
    ds_metrics_reset_all();

    /* 2. インクリメント＆取得 */
    ds_metrics_increment("test.counter");
    ds_metrics_increment("test.counter");
    int64_t v = ds_metrics_get("test.counter");
    DS_TEST_ASSERT(v == 2, "counter == 2");

    ds_metrics_increment("other");
    DS_TEST_ASSERT(ds_metrics_get("other") == 1, "other == 1");

    /* 3. 合算チェック：get_total() がないので個別取得で合計 */
    int64_t total =
        ds_metrics_get("test.counter")
      + ds_metrics_get("other");
    DS_TEST_ASSERT(total == 3, "total == 3 (sum)");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__metrics_basic 完了");
}