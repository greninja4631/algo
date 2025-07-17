/**
 * @file test_memory.c
 * @brief メモリ管理(memory)モジュールのテスト
 */
#include "util/test_memory.h"
#include "util/logger.h"
#include "util/memory.h"

#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

/**
 * @brief memory 基本動作テスト
 */
void test__memory_basic(void)
{
    void* ptr = ds_malloc(128);
    DS_TEST_ASSERT(ptr != NULL, "malloc: not NULL");
    ds_free(ptr);
    DS_TEST_ASSERT(1, "free: no crash");
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__memory_basic 完了");
}

/**
 * @brief memory エッジケーステスト
 */
void test__memory_edge_cases(void)
{
    ds_free(NULL); // NULL free許容など
    DS_TEST_ASSERT(1, "free: NULL安全（クラッシュしない）");
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__memory_edge_cases 完了");
}
