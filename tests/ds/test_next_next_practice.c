/**
 * @file    tests/ds/test_next_next_practice.c
 * @brief   ds_next_next_practice モジュール単体テスト
 * @note    main() は tests/test_main.c 側で集約
 *
 * 2025-07 ガイドライン（Opaque 型 + 抽象アロケータ DI）準拠
 */
#include <stdlib.h>                     /* calloc / free                       */
#include "ds/next_next_practice.h"      /* 被テスト API                        */
#include "ds/test_next_next_practice.h" /* テスト用 helper (マクロなど)         */
#include "util/logger.h"                /* ds_log                              */

/*─────────────────────────────────────*
 * 1. 標準 malloc/free → 抽象アロケータ
 *─────────────────────────────────────*/
static void* _std_alloc(size_t n, size_t sz) { return calloc(n, sz); }
static void  _std_free (void* p)             { free(p);             }

static const ds_allocator_t g_alloc_impl = {
    .alloc = _std_alloc,
    .free  = _std_free
};
#define G_ALLOC (&g_alloc_impl)

/*─────────────────────────────────────*
 * 2. 共通アサート
 *─────────────────────────────────────*/
#define TASSERT(cond, msg)                                                      \
    do {                                                                        \
        if (cond)                                                               \
            ds_log(DS_LOG_LEVEL_INFO , "[PASS] %s", (msg));                     \
        else                                                                    \
            ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",                     \
                   (msg), __FILE__, __LINE__);                                  \
    } while (0)

/*======================================================================*
 *  3-A. 基本機能テスト
 *======================================================================*/
void test__next_next_practice_basic(void)
{
    ds_next_next_practice_t *obj = NULL;

    /* create */
    TASSERT(ds_next_next_practice_create(G_ALLOC, &obj) == DS_SUCCESS && obj,
            "create");

    /* 公開 API 動作確認 */
    TASSERT(ds_next_next_practice_run_all_demos(obj) == DS_SUCCESS,
            "run_all_demos");

    /* destroy */
    TASSERT(ds_next_next_practice_destroy(G_ALLOC, obj) == DS_SUCCESS,
            "destroy");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__next_next_practice_basic finished");
}

/*======================================================================*
 *  3-B. 異常系・NULL 安全テスト
 *======================================================================*/
void test__next_next_practice_edge_cases(void)
{
    /* NULL 破棄は no-op */
    TASSERT(ds_next_next_practice_destroy(G_ALLOC, NULL) == DS_SUCCESS,
            "destroy NULL");

    /* NULL out-ptr */
    TASSERT(ds_next_next_practice_create(G_ALLOC, NULL) == DS_ERR_NULL_POINTER,
            "create NULL out_ptr");

    /* NULL ハンドル */
    TASSERT(ds_next_next_practice_run_all_demos(NULL) == DS_ERR_NULL_POINTER,
            "run_all_demos NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__next_next_practice_edge_cases finished");
}
