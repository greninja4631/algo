#include <stdint.h>
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