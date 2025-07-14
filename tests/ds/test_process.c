/**
 * @file  tests/ds/test_process.c
 * @brief process モジュールの単体テスト
 * @details  main() は tests/test_main.c 側で一括呼び出し
 */

#include <stdio.h>
#include <assert.h>

#include "data_structures.h"   /* ds_error_t など共通型          */
#include "ds/process.h"        /* ds_process_* API               */

/* ──────────────────── 最小限アサート ─────────────────── */
#define DS_TEST_ASSERT(cond, msg)                                      \
    do {                                                               \
        if (!(cond)) { printf("[FAIL] %s\n", msg); assert(0); }        \
        else          { printf("[PASS] %s\n", msg); }                  \
    } while (0)

/* ──────────────────── テストケース ─────────────────── */
/* 基本動作テスト */
void test__process_basic(void)
{
    ds_process_t *proc = NULL;

    /* create */
    ds_error_t err = ds_process_create(123, 45, &proc);
    DS_TEST_ASSERT(err == DS_SUCCESS,                "create: DS_SUCCESS");
    DS_TEST_ASSERT(proc != NULL,                     "create: not NULL");

    /* ゲッターで検証（メンバ直読みは禁止） */
    DS_TEST_ASSERT(ds_process_get_id(proc) == 123,   "id matches");
    DS_TEST_ASSERT(ds_process_get_burst_time(proc) == 45,
                   "burst_time matches");

    /* destroy は void 戻り値 ─ 例外やクラッシュが起きないことを確認 */
    ds_process_destroy(proc);
}

/* 境界条件テスト */
void test__process_edge_cases(void)
{
    /* NULL ポインタ渡し */
    ds_process_destroy(NULL);    /* 何も起きなければ PASS */

    /* out_process==NULL */
    DS_TEST_ASSERT(ds_process_create(1, 2, NULL) == DS_ERR_NULL_POINTER,
                   "create: NULL out_process");
}