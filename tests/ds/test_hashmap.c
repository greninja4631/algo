/**
 * @file    tests/ds/test_hashmap.c
 * @brief   ds_hashmap モジュール単体テスト
 * @details main() は tests/test_main.c 側で集約。
 */

#include <stdlib.h>          /* calloc / free */
#include "ds/hashmap.h"
#include "util/logger.h"

/*─────────────────────────────────────*
 * 1. 標準 malloc/free を ds_allocator_t
 *─────────────────────────────────────*/
static void* _std_alloc(size_t cnt, size_t sz) { return calloc(cnt, sz); }
static void  _std_free (void* p)               { free(p);               }

static const ds_allocator_t g_alloc_impl = {
    .alloc = _std_alloc,
    .free  = _std_free
};
#define G_ALLOC (&g_alloc_impl)

/*───────────────────────────────*
 * 2. 共通アサーションマクロ
 *───────────────────────────────*/
#define TASSERT(cond, msg)                                                      \
    do {                                                                        \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO , "[PASS] %s", (msg)); }           \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",             \
                           (msg), __FILE__, __LINE__); }                        \
    } while (0)

/* 要素破棄ダミー (今回は解放不要なので no-op) */
static void dummy_free(void* p) { (void)p; }

/*───────────────────────────────*
 * 3. 基本機能テスト
 *───────────────────────────────*/
void test__hashmap_basic(void)
{
    ds_error_t   err;
    ds_hashmap_t *map  = NULL;
    void         *val  = NULL;

    const char *k1 = "apple";
    const char *k2 = "banana";
    const char *k3 = "orange";
    int v1 = 111, v2 = 222, v3 = 333;

    /* ――― create ――― */
    err = ds_hashmap_create(G_ALLOC, 16, dummy_free, dummy_free, &map);
    TASSERT(err == DS_SUCCESS && map, "create");

    TASSERT(ds_hashmap_size(map) == 0, "size == 0 after create");

    /* ――― put / get ――― */
    TASSERT(ds_hashmap_put(G_ALLOC, map, k1, &v1) == DS_SUCCESS, "put k1");
    TASSERT(ds_hashmap_put(G_ALLOC, map, k2, &v2) == DS_SUCCESS, "put k2");
    TASSERT(ds_hashmap_put(G_ALLOC, map, k3, &v3) == DS_SUCCESS, "put k3");
    TASSERT(ds_hashmap_size(map) == 3, "size == 3");

    TASSERT(ds_hashmap_get(map, k1, &val) == DS_SUCCESS && val == &v1, "get k1");
    TASSERT(ds_hashmap_get(map, k2, &val) == DS_SUCCESS && val == &v2, "get k2");
    TASSERT(ds_hashmap_get(map, k3, &val) == DS_SUCCESS && val == &v3, "get k3");

    /* ――― remove ――― */
    TASSERT(ds_hashmap_remove(G_ALLOC, map, k2) == DS_SUCCESS, "remove k2");
    TASSERT(ds_hashmap_size(map) == 2, "size == 2");

    /* remove nonexistent */
    TASSERT(ds_hashmap_remove(G_ALLOC, map, "zzz") != DS_SUCCESS,
            "remove non-existent key");

    /* get after remove */
    TASSERT(ds_hashmap_get(map, k2, &val) != DS_SUCCESS, "get k2 after remove");

    /* ――― NULL safety ――― */
    TASSERT(ds_hashmap_put   (G_ALLOC, NULL, k1, &v1)  == DS_ERR_NULL_POINTER, "put NULL");
    TASSERT(ds_hashmap_remove(G_ALLOC, NULL, k1)       == DS_ERR_NULL_POINTER, "rm  NULL");
    TASSERT(ds_hashmap_get   (NULL, k1, &val)          == DS_ERR_NULL_POINTER, "get NULL");
    TASSERT(ds_hashmap_size  (NULL)                    == 0,                  "size NULL");

    /* ――― destroy ――― */
    TASSERT(ds_hashmap_destroy(G_ALLOC, map) == DS_SUCCESS, "destroy");
    TASSERT(ds_hashmap_destroy(G_ALLOC, NULL) == DS_ERR_NULL_POINTER,
            "destroy NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__hashmap_basic finished");
}
