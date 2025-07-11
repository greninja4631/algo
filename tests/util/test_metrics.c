/**
 * @file test_metrics.c
 * @brief メトリクス(metrics)モジュールのテスト
 */
#include "util/test_metrics.h"
#include "util/logger.h"
#include "util/metrics.h"

#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

/**
 * @brief metrics 基本動作テスト
 */
void test__metrics_basic(void)
{
    // 仮API：ds_metrics_increment, ds_metrics_getなど
    ds_metrics_reset_all();
    ds_metrics_increment("test.counter");
    int64_t val = ds_metrics_get("test.counter");
    DS_TEST_ASSERT(val == 1, "increment: +1");
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__metrics_basic 完了");
}

/**
 * @brief metrics エッジケーステスト
 */
void test__metrics_edge_cases(void)
{
    int64_t val = ds_metrics_get("not.exist");
    DS_TEST_ASSERT(val == 0, "get: 未登録カウンタは0");
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__metrics_edge_cases 完了");
}