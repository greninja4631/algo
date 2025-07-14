/**
 * @file   tests/ds/test_next_next_practice.c
 * @brief  ds_next_next_practice モジュール単体テスト
 *
 * 2025-07 ガイドライン〈Opaque 型 + マイクロアーキテクチャ〉準拠
 */

#include "ds/next_next_practice.h"       /* 本番 API 宣言 */
#include "ds/test_next_next_practice.h"  /* テスト宣言ヘッダ */
#include "util/logger.h"                 /* ds_log() */
#include "util/test_util.h"              /* 共通マクロがあれば */

/* ──────────── 共通アサーション ──────────── */
#define DS_TEST_ASSERT(cond, msg)                                           \
    do {                                                                    \
        if (cond) {                                                         \
            ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", (msg));                 \
        } else {                                                            \
            ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",                 \
                   (msg), __FILE__, __LINE__);                              \
        }                                                                   \
    } while (0)

/* ─────────────────────────────────────────────
 * 1. 基本動作テスト
 * ────────────────────────────────────────────*/
void test__next_next_practice_basic(void)
{
    ds_next_next_practice_t *obj = NULL;

    /* 生成：成功コード＋out_ptr 形式 */
    DS_TEST_ASSERT(
        ds_next_next_practice_create(&obj) == DS_SUCCESS && obj,
        "create");

    /* 公開デモ API が正常終了すること */
    DS_TEST_ASSERT(
        ds_next_next_practice_run_all_demos(obj) == DS_SUCCESS,
        "run_all_demos");

    /* 破棄：クラッシュしないことを確認 */
    ds_next_next_practice_destroy(obj);
    DS_TEST_ASSERT(1, "destroy: no crash");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__next_next_practice_basic 完了");
}

/* ─────────────────────────────────────────────
 * 2. 異常系／NULL 安全性テスト
 * ────────────────────────────────────────────*/
void test__next_next_practice_edge_cases(void)
{
    /* NULL 破棄は no-op で安全 */
    ds_next_next_practice_destroy(NULL);
    DS_TEST_ASSERT(1, "destroy: NULL safety");

    /* NULL out_ptr での生成は DS_ERR_NULL_POINTER を返す */
    DS_TEST_ASSERT(
        ds_next_next_practice_create(NULL) == DS_ERR_NULL_POINTER,
        "create: NULL out_ptr");

    /* NULL ハンドルでのデモ実行は DS_ERR_NULL_POINTER を返す */
    DS_TEST_ASSERT(
        ds_next_next_practice_run_all_demos(NULL) == DS_ERR_NULL_POINTER,
        "run_all_demos: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__next_next_practice_edge_cases 完了");
}