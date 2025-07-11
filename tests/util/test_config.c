/**
 * @file test_config.c
 * @brief 設定(config)モジュールのテスト
 */
#include <stddef.h>            // NULL, size_t
#include "util/test_config.h"
#include "util/logger.h"
#include "util/config.h"

#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

/**
 * @brief config 基本動作テスト
 */
void test__config_basic(void)
{
    ds_config_t* config = ds_config_create();
    DS_TEST_ASSERT(config != NULL, "create: config not NULL");
    ds_config_destroy(config);
    DS_TEST_ASSERT(1, "destroy: no crash");
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__config_basic 完了");
}

/**
 * @brief config エッジケーステスト
 */
void test__config_edge_cases(void)
{
    ds_config_destroy(NULL); // NULL破棄
    DS_TEST_ASSERT(1, "destroy: NULL安全（クラッシュしない）");
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__config_edge_cases 完了");
}