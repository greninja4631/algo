#include <stddef.h>
#include "util/config.h"
#include "util/logger.h"
#include "data_structures.h"  // ds_allocator_t

// テスト用：ゼロクリア＋標準freeで十分な場合のダミーアロケータ
static void* test_alloc(size_t count, size_t size) { return calloc(count, size); }
static void  test_free(void* ptr)                 { free(ptr); }
static ds_allocator_t test_allocator = { test_alloc, test_free };

#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", (msg)); \
        else      ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", (msg), __FILE__, __LINE__); \
    } while (0)

/* 基本動作テスト */
void test__config_basic(void)
{
    ds_config_t *cfg = NULL;
    ds_error_t rc = ds_config_create(&test_allocator, &cfg);    // alloc明示
    DS_TEST_ASSERT(rc == DS_SUCCESS, "create OK");
    DS_TEST_ASSERT(cfg != NULL,      "cfg != NULL");

    ds_config_destroy(&test_allocator, cfg);                    // alloc明示
    DS_TEST_ASSERT(1, "destroy: no crash");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__config_basic 完了");
}

/* エッジケーステスト */
void test__config_edge_cases(void)
{
    ds_config_destroy(&test_allocator, NULL);   // NULLでもクラッシュしないこと
    DS_TEST_ASSERT(1, "destroy: NULL safe");

    ds_config_t *cfg = NULL;
    ds_error_t rc = ds_config_create(NULL, &cfg);  // alloc=NULLは必ずエラー
    DS_TEST_ASSERT(rc == DS_ERR_NULL_POINTER, "create: NULL alloc");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__config_edge_cases 完了");
}
