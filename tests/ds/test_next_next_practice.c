#include <stddef.h>
/**
 * @file    tests/ds/test_next_next_practice.c
 * @brief   ds_next_next_practice ― ユニットテスト
 * @note    test_main.c から呼び出される（単体実行はしない）
 * @guideline 2025-07 (Opaque + DI + Valgrind-clean)
 */
#include <stdlib.h>          /* calloc / free            */
#include <assert.h>
#include "ds/next_next_practice.h"      /* 被テスト API  */
#include "ds/test_next_next_practice.h" /* テスト宣言     */
#include "util/logger.h"                /* ds_log         */

/*───────────────────────────────────
 * 1. テスト専用アロケータ（calloc/free → DI）
 *───────────────────────────────────*/
static void* _std_alloc(size_t n, size_t sz) { return n ? calloc(n, sz) : NULL; }
static void  _std_free (void* p)             { if (p) free(p); }

static const ds_allocator_t g_alloc_impl = {
    .alloc = _std_alloc,
    .free  = _std_free
};
#define G_ALLOC (&g_alloc_impl)

/*───────────────────────────────────
 * 2. アサーションマクロ（6段ログ）
 *───────────────────────────────────*/
#define TASSERT(cond, msg)                                                   \
    do {                                                                     \
        if (cond) {                                                          \
            ds_log(DS_LOG_LEVEL_INFO , "[PASS] %s", (msg));                  \
        } else {                                                             \
            ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",                  \
                   (msg), __FILE__, __LINE__);                               \
            assert(0);                                                       \
        }                                                                    \
    } while (0)

/*======================================================================*
 * 3-A. 基本機能テスト
 *======================================================================*/
void test__next_next_practice_basic(void)
{
    ds_next_next_practice_t *obj = NULL;

    /* create */
    TASSERT(ds_next_next_practice_create(G_ALLOC, &obj) == DS_SUCCESS && obj,
            "create");

    /* run demos – 必ずアロケータ渡す（2引数） */
    TASSERT(ds_next_next_practice_run_all_demos(G_ALLOC, obj) == DS_SUCCESS,
            "run_all_demos");

    /* destroy */
    TASSERT(ds_next_next_practice_destroy(G_ALLOC, obj) == DS_SUCCESS,
            "destroy");
}

void test__next_next_practice_edge_cases(void)
{
    /* destroy(NULL) は no-op → DS_SUCCESS */
    TASSERT(ds_next_next_practice_destroy(G_ALLOC, NULL) == DS_SUCCESS,
            "destroy NULL");

    /* create() – out_ptr NULL */
    TASSERT(ds_next_next_practice_create(G_ALLOC, NULL) == DS_ERR_NULL_POINTER,
            "create NULL out_ptr");

    /* run_all_demos(NULL) → NULL エラー
       ↓ 必ず第1引数 G_ALLOC（2引数） */
    TASSERT(ds_next_next_practice_run_all_demos(G_ALLOC, NULL) == DS_ERR_NULL_POINTER,
            "run_all_demos NULL");
}