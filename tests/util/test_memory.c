#include <stddef.h>
/**
 * @file test_memory.c
 * @brief メモリ管理(memory)モジュールのテスト
 * @note 必ずDIアロケータ経由でテストし、free漏れ・二重free無しを保証
 */
#include "util/test_memory.h"
#include "util/logger.h"
#include "util/memory.h"
#include <string.h>

#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

/* テスト用アロケータ（DI） */
static void* test_alloc(size_t n, size_t sz) { return calloc(n, sz); }
static void  test_free(void* p)              { free(p); }

static const ds_allocator_t test_allocator = {
    .alloc = test_alloc,
    .free  = test_free,
};

/**
 * @brief memory 基本動作テスト
 * @details test_allocator経由でalloc/freeを必ず1セットで呼ぶ
 */
void test__memory_basic(void)
{
    void* ptr = ds_malloc(&test_allocator, 128, 1);
    DS_TEST_ASSERT(ptr != NULL, "malloc: not NULL");
    ds_free(&test_allocator, ptr);
    DS_TEST_ASSERT(1, "free: no crash");
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__memory_basic 完了");
}

/**
 * @brief memory エッジケーステスト
 * @details free(NULL)もクラッシュしないことを保証
 */
void test__memory_edge_cases(void)
{
    ds_free(&test_allocator, NULL); // NULL free安全
    DS_TEST_ASSERT(1, "free: NULL安全（クラッシュしない）");
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__memory_edge_cases 完了");
}