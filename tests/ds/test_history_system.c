/**
 * @file    tests/ds/test_history_system.c
 * @brief   ds_history_system_t ― Undo/Redo 履歴管理モジュール単体テスト
 * @note    main() は tests/test_main.c が担う
 */

#include <stdlib.h>               /* calloc / free */
#include "ds/history_system.h"
#include "ds/test_history_system.h"   /* TEST_CMD_PTR, test_command_init */
#include "util/logger.h"

/*─────────────────────────────────────*
 * 標準calloc/free→ds_allocator_t経由でDI
 *─────────────────────────────────────*/
static void* _std_alloc(size_t cnt, size_t sz) { return cnt ? calloc(cnt, sz) : NULL; }
static void  _std_free(void* p)                { if (p) free(p); }

static const ds_allocator_t g_alloc_impl = {
    .alloc = _std_alloc,
    .free  = _std_free
};
#define G_ALLOC (&g_alloc_impl)

/*─────────────────────────────────────*
 * プロジェクト共通アサーションマクロ
 *─────────────────────────────────────*/
#define DS_TEST_ASSERT(cond, msg)                                              \
    do {                                                                       \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO , "[PASS] %s", (msg)); }          \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",            \
                           (msg), __FILE__, __LINE__); }                       \
    } while (0)

/*─────────────────────────────────────*
 * 基本動作テスト
 *─────────────────────────────────────*/
void test__history_system_basic(void)
{
    ds_error_t           err;
    ds_history_system_t *hist = NULL;

    /* create (最大 3 コマンドまで保持) */
    err = ds_history_system_create(G_ALLOC, 3, &hist);
    DS_TEST_ASSERT(err == DS_SUCCESS && hist, "create");

    /* dummy commands */
    ds_command_t cmd1, cmd2, cmd3;
    test_command_init(&cmd1, 1, 10);
    test_command_init(&cmd2, 2, 20);
    test_command_init(&cmd3, 3, 30);

    /* execute */
    DS_TEST_ASSERT(
        ds_history_system_execute_command(G_ALLOC, hist,
                                          TEST_CMD_PTR(&cmd1)) == DS_SUCCESS,
        "execute cmd1");
    DS_TEST_ASSERT(
        ds_history_system_execute_command(G_ALLOC, hist,
                                          TEST_CMD_PTR(&cmd2)) == DS_SUCCESS,
        "execute cmd2");
    DS_TEST_ASSERT(
        ds_history_system_execute_command(G_ALLOC, hist,
                                          TEST_CMD_PTR(&cmd3)) == DS_SUCCESS,
        "execute cmd3");

    /* can_undo / can_redo フラグ確認 */
    DS_TEST_ASSERT(ds_history_system_can_undo(hist), "can_undo == true");
    DS_TEST_ASSERT(!ds_history_system_can_redo(hist), "can_redo == false");

    /* undo / redo */
    DS_TEST_ASSERT(ds_history_system_undo(G_ALLOC, hist) == DS_SUCCESS, "undo #1");
    DS_TEST_ASSERT(ds_history_system_redo(G_ALLOC, hist) == DS_SUCCESS, "redo #1");

    /* clear → すべて undo 不可 */
    DS_TEST_ASSERT(ds_history_system_clear(G_ALLOC, hist) == DS_SUCCESS, "clear");
    DS_TEST_ASSERT(!ds_history_system_can_undo(hist), "can_undo == false after clear");

    /* destroy */
    DS_TEST_ASSERT(ds_history_system_destroy(G_ALLOC, hist) == DS_SUCCESS, "destroy");

    /* NULL 安全性 */
    DS_TEST_ASSERT(ds_history_system_destroy(G_ALLOC, NULL) == DS_ERR_NULL_POINTER,
                   "destroy(NULL)");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__history_system_basic finished");
}

/* 必要になったら Edge-Case / Stress テスト関数を追記 */