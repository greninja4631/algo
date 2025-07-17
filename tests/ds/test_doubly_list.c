/**
 * @file    tests/ds/test_doubly_list.c
 * @brief   ds_doubly_list モジュール単体テスト
 * @details main() は tests/test_main.c 側で集約。
 */

#include <stdlib.h>               /* calloc / free */
#include "ds/doubly_list.h"       /* 被テスト API */
#include "util/logger.h"          /* ds_log */

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
 * 2. 汎用アサーションマクロ
 *───────────────────────────────*/
#define TASSERT(c, m)                                                         \
    do {                                                                      \
        if (c) ds_log(DS_LOG_LEVEL_INFO , "[PASS] %s", (m));                  \
        else   ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",                \
                      (m), __FILE__, __LINE__);                               \
    } while (0)

/*───────────────────────────────*
 * 3-A. 基本機能テスト
 *───────────────────────────────*/
void test__doubly_list_basic(void)
{
    ds_doubly_list_t *list = NULL;
    int a = 10, b = 20, c = 30, d = 40;
    void *tmp = NULL;

    /* Create */
    TASSERT(ds_doubly_list_create(G_ALLOC, &list) == DS_SUCCESS, "create");
    TASSERT(list != NULL,                               "list != NULL");
    TASSERT(ds_doubly_list_is_empty(list),              "empty after create");
    TASSERT(ds_doubly_list_size(list) == 0,             "size 0");

    /* Push back (a,b) */
    TASSERT(ds_doubly_list_insert_back (G_ALLOC, list, &a) == DS_SUCCESS, "push_back a");
    TASSERT(ds_doubly_list_insert_back (G_ALLOC, list, &b) == DS_SUCCESS, "push_back b");
    TASSERT(ds_doubly_list_size(list) == 2,                               "size 2");

    /* Push front (c) */
    TASSERT(ds_doubly_list_insert_front(G_ALLOC, list, &c) == DS_SUCCESS, "push_front c");
    TASSERT(ds_doubly_list_size(list) == 3,                               "size 3");

    /* Insert at idx=1 (d)  c d a b */
    TASSERT(ds_doubly_list_insert_at   (G_ALLOC, list, 1, &d) == DS_SUCCESS,"insert_at d");
    TASSERT(ds_doubly_list_size(list) == 4,                                 "size 4");

    /* Validate order */
    TASSERT(ds_doubly_list_get_at(list, 0, &tmp) == DS_SUCCESS && *(int*)tmp == c, "get 0=c");
    TASSERT(ds_doubly_list_get_at(list, 1, &tmp) == DS_SUCCESS && *(int*)tmp == d, "get 1=d");
    TASSERT(ds_doubly_list_get_at(list, 2, &tmp) == DS_SUCCESS && *(int*)tmp == a, "get 2=a");
    TASSERT(ds_doubly_list_get_at(list, 3, &tmp) == DS_SUCCESS && *(int*)tmp == b, "get 3=b");

    /* pop front (c) */
    TASSERT(ds_doubly_list_remove_front(G_ALLOC, list, &tmp) == DS_SUCCESS && *(int*)tmp == c,
            "remove_front c");
    /* pop back (b)   d a */
    TASSERT(ds_doubly_list_remove_back (G_ALLOC, list, &tmp) == DS_SUCCESS && *(int*)tmp == b,
            "remove_back b");
    /* remove_at idx=1 (a)  d */
    TASSERT(ds_doubly_list_remove_at   (G_ALLOC, list, 1, &tmp) == DS_SUCCESS && *(int*)tmp == a,
            "remove_at 1 a");
    /* remove_at idx=0 (d)  EMPTY */
    TASSERT(ds_doubly_list_remove_at   (G_ALLOC, list, 0, &tmp) == DS_SUCCESS && *(int*)tmp == d,
            "remove_at 0 d");
    TASSERT(ds_doubly_list_is_empty(list), "empty after clears");

    /* remove on empty */
    TASSERT(ds_doubly_list_remove_front(G_ALLOC, list, &tmp) == DS_ERR_EMPTY,
            "remove_front empty");

    /* Destroy */
    TASSERT(ds_doubly_list_destroy(G_ALLOC, list) == DS_SUCCESS, "destroy OK");
}

/*───────────────────────────────*
 * 3-B. 境界・異常系テスト
 *───────────────────────────────*/
void test__doubly_list_edge_cases(void)
{
    void *tmp = NULL;

    /* NULL list param */
    TASSERT(ds_doubly_list_insert_back (G_ALLOC, NULL, &tmp) == DS_ERR_NULL_POINTER,"ins_back NULL");
    TASSERT(ds_doubly_list_remove_back (G_ALLOC, NULL, &tmp) == DS_ERR_NULL_POINTER,"rm_back NULL");
    TASSERT(ds_doubly_list_get_at(NULL, 0, &tmp)              == DS_ERR_NULL_POINTER,"get_at NULL");

    /* Index out-of-range */
    ds_doubly_list_t *list = NULL;
    TASSERT(ds_doubly_list_create(G_ALLOC, &list) == DS_SUCCESS, "create for OOR");
    TASSERT(ds_doubly_list_insert_back(G_ALLOC, list, &tmp) == DS_SUCCESS, "insert OOR");
    TASSERT(ds_doubly_list_get_at(list, 99, &tmp)  == DS_ERR_OUT_OF_RANGE, "get_at OOR");
    TASSERT(ds_doubly_list_remove_at(G_ALLOC, list, 99, &tmp) == DS_ERR_OUT_OF_RANGE,
            "remove_at OOR");
    ds_doubly_list_destroy(G_ALLOC, list);
}

/*───────────────────────────────*
 * 3-C. メモリ負荷テスト (256 要素)
 *───────────────────────────────*/
void test__doubly_list_bulk(void)
{
    ds_doubly_list_t *list = NULL;
    int buf[256];
    void *tmp = NULL;

    TASSERT(ds_doubly_list_create(G_ALLOC, &list) == DS_SUCCESS, "create bulk");

    for (size_t i = 0; i < 256; ++i) {
        buf[i] = (int)i;
        TASSERT(ds_doubly_list_insert_back(G_ALLOC, list, &buf[i]) == DS_SUCCESS, "bulk push");
    }
    TASSERT(ds_doubly_list_size(list) == 256, "bulk size == 256");

    for (size_t i = 0; i < 256; ++i)
        TASSERT(ds_doubly_list_remove_front(G_ALLOC, list, &tmp) == DS_SUCCESS, "bulk pop");

    TASSERT(ds_doubly_list_is_empty(list), "bulk empty");

    ds_doubly_list_destroy(G_ALLOC, list);
}
