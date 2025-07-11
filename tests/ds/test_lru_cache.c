/**
 * @file test_lru_cache.c
 * @brief ds_lru_cache_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 */

#include "ds/lru_cache.h"
#include "util/test_util.h"
#include "util/logger.h"

// テスト用アサーションマクロ（共通化されている場合はそちらを利用）
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

// LRUキャッシュ基本動作テスト
void ds_test_lru_cache_basic(void)
{
    ds_error_t err;
    ds_lru_cache_t* cache = NULL;
    void* value = NULL;
    size_t size = 0;
    int v1 = 100, v2 = 200, v3 = 300;

    // --- 生成 ---
    err = ds_lru_cache_create(2, &cache);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(cache != NULL, "create: not NULL");

    // --- 追加 ---
    err = ds_lru_cache_put(cache, "a", &v1);
    DS_TEST_ASSERT(err == DS_SUCCESS, "put: a");
    err = ds_lru_cache_put(cache, "b", &v2);
    DS_TEST_ASSERT(err == DS_SUCCESS, "put: b");

    // --- 検索 ---
    err = ds_lru_cache_get(cache, "a", &value);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)value == v1, "get: a (hit)");
    err = ds_lru_cache_get(cache, "b", &value);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)value == v2, "get: b (hit)");

    // --- キャッシュサイズ確認 ---
    err = ds_lru_cache_size(cache, &size);
    DS_TEST_ASSERT(err == DS_SUCCESS && size == 2, "size: after put 2");

    // --- 上限超過: "c"追加で"b"か"a"どちらかがLRUで消える ---
    err = ds_lru_cache_put(cache, "c", &v3);
    DS_TEST_ASSERT(err == DS_SUCCESS, "put: c");

    err = ds_lru_cache_size(cache, &size);
    DS_TEST_ASSERT(err == DS_SUCCESS && size == 2, "size: after evict 1");

    // --- LRUチェック（"a"が消える仕様の場合） ---
    err = ds_lru_cache_get(cache, "a", &value);
    DS_TEST_ASSERT(err != DS_SUCCESS, "get: a (evicted)");
    err = ds_lru_cache_get(cache, "b", &value);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)value == v2, "get: b (still present)");
    err = ds_lru_cache_get(cache, "c", &value);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)value == v3, "get: c (just inserted)");

    // --- NULL安全 ---
    err = ds_lru_cache_put(NULL, "x", &v1);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "put: NULL");
    err = ds_lru_cache_get(NULL, "x", &value);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "get: NULL");
    err = ds_lru_cache_size(NULL, &size);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "size: NULL");

    // --- 破棄 ---
    err = ds_lru_cache_destroy(cache);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
    DS_TEST_ASSERT(ds_lru_cache_destroy(NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_lru_cache_basic 完了");
}