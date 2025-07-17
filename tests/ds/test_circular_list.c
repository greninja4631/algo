/**
 * @file    tests/ds/test_circular_list.c
 * @brief   ds_circular_list モジュールの単体テスト
 * @details main() は tests/test_main.c で一括管理。
 */

#include <stdlib.h>             /* calloc / free */
#include "ds/circular_list.h"   /* 被テスト API */
#include "util/logger.h"        /* ds_log */

/*─────────────────────────────────────*
 * 1. 標準 malloc/free を ds_allocator_t
 *─────────────────────────────────────*/
static void* _std_alloc(size_t cnt, size_t size) { return calloc(cnt, size); }
static void  _std_free (void*  p)                { free(p);                }

static const ds_allocator_t g_alloc_impl = {
    .alloc = _std_alloc,
    .free  = _std_free
};
#define G_ALLOC (&g_alloc_impl)

/*───────────────────────────────*
 * 2. 共通アサーションマクロ
 *───────────────────────────────*/
#define TASSERT(cond, msg)                                                     \
    do {                                                                       \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", (msg)); }          \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",            \
                           (msg), __FILE__, __LINE__); }                       \
    } while (0)

/*───────────────────────────────*
 * 3. 基本動作テスト
 *───────────────────────────────*/
void test__circular_list_basic(void)
{
    ds_circular_list_t *list = NULL;
    void *tmp = NULL;
    int a = 10, b = 20, c = 30;

    /* Create */
    TASSERT(ds_circular_list_create(G_ALLOC, &list) == DS_SUCCESS, "create");
    TASSERT(list != NULL,                                "list != NULL");
    TASSERT(ds_circular_list_is_empty(list),             "empty after create");
    TASSERT(ds_circular_list_size(list) == 0,            "size 0");

    /* Insert */
    TASSERT(ds_circular_list_insert(G_ALLOC, list, &a) == DS_SUCCESS, "insert a");
    TASSERT(ds_circular_list_insert(G_ALLOC, list, &b) == DS_SUCCESS, "insert b");
    TASSERT(ds_circular_list_insert(G_ALLOC, list, &c) == DS_SUCCESS, "insert c");
    TASSERT(ds_circular_list_size(list) == 3,                         "size 3");

    /* Remove (FIFO with rotation) */
    TASSERT(ds_circular_list_remove(G_ALLOC, list, &tmp) == DS_SUCCESS && *(int*)tmp == a,
            "remove a");
    TASSERT(ds_circular_list_remove(G_ALLOC, list, &tmp) == DS_SUCCESS && *(int*)tmp == b,
            "remove b");
    TASSERT(ds_circular_list_remove(G_ALLOC, list, &tmp) == DS_SUCCESS && *(int*)tmp == c,
            "remove c");
    TASSERT(ds_circular_list_is_empty(list), "empty after all remove");

    /* Remove on empty */
    TASSERT(ds_circular_list_remove(G_ALLOC, list, &tmp) == DS_ERR_EMPTY,
            "remove empty");

    /* Destroy */
    TASSERT(ds_circular_list_destroy(G_ALLOC, list) == DS_SUCCESS, "destroy OK");

    /* NULL-safety checks */
    TASSERT(ds_circular_list_is_empty(NULL),               "is_empty(NULL)");
    TASSERT(ds_circular_list_size(NULL) == 0,              "size(NULL)");
    TASSERT(ds_circular_list_destroy(G_ALLOC, NULL) == DS_ERR_NULL_POINTER,
            "destroy(NULL)");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__circular_list_basic finished");
}
