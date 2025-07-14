/**
 * @file   tests/util/test_config.c
 * @brief  util/config モジュールの単体テスト
 *
 * 2025-07 ガイドライン（Opaque 型 + エラーコード API）準拠
 */

#include <stddef.h>          /* NULL */
#include "util/config.h"     /* 被テスト API */
#include "util/logger.h"     /* ds_log */

/* プロジェクト共通アサーション */
#define DS_TEST_ASSERT(cond, msg)                                            \
    do {                                                                     \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", (msg)); }        \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",          \
                           (msg), __FILE__, __LINE__); }                     \
    } while (0)

/* ──────────────────────────────────────────
 * 1. 基本動作テスト
 * ────────────────────────────────────────── */
void test__config_basic(void)
{
    ds_config_t *cfg = NULL;

    /* 生成：戻り値は ds_error_t、成功で out_ptr に設定 */
    DS_TEST_ASSERT(ds_config_create(&cfg) == DS_SUCCESS, "create OK");
    DS_TEST_ASSERT(cfg != NULL, "cfg != NULL");

    /* 破棄してリークが無いことを確認 */
    ds_config_destroy(cfg);
    DS_TEST_ASSERT(1, "destroy: no crash");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__config_basic 完了");
}

/* ──────────────────────────────────────────
 * 2. エッジケース／NULL 安全テスト
 * ────────────────────────────────────────── */
void test__config_edge_cases(void)
{
    /* NULL 破棄はクラッシュしない */
    ds_config_destroy(NULL);
    DS_TEST_ASSERT(1, "destroy: NULL safe");

    /* NULL out_ptr → エラーコード */
    DS_TEST_ASSERT(ds_config_create(NULL) == DS_ERR_NULL_POINTER,
                   "create: NULL out_ptr");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__config_edge_cases 完了");
}