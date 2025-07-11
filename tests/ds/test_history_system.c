/**
 * @file test_history_system.c
 * @brief ds_history_system_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出される。main関数は持たない。
 */


#include "ds/history_system.h"
#include "util/test_util.h" 
#include "util/logger.h"
// テスト用コマンド型・初期化を共通化

// テスト用アサーションマクロ
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

void ds_test_history_system_basic(void)
{
    ds_error_t err;
    ds_history_system_t* hist = ds_history_system_create(3);

    // --- 生成検証 ---
    DS_TEST_ASSERT(hist != NULL, "create: not NULL");
    DS_TEST_ASSERT(!ds_history_system_can_undo(hist), "can_undo: false after create");
    DS_TEST_ASSERT(!ds_history_system_can_redo(hist), "can_redo: false after create");

    // --- コマンド登録 ---
    test_command_t cmd1, cmd2, cmd3;
    test_command_init(&cmd1, 1, 10);
    test_command_init(&cmd2, 2, 20);
    test_command_init(&cmd3, 3, 30);

    err = ds_history_system_execute_command(hist, TEST_CMD_PTR(&cmd1));
    DS_TEST_ASSERT(err == DS_SUCCESS, "execute_command: 1");
    DS_TEST_ASSERT(ds_history_system_can_undo(hist), "can_undo: after exec 1");
    DS_TEST_ASSERT(!ds_history_system_can_redo(hist), "can_redo: after exec 1");

    err = ds_history_system_execute_command(hist, TEST_CMD_PTR(&cmd2));
    DS_TEST_ASSERT(err == DS_SUCCESS, "execute_command: 2");
    err = ds_history_system_execute_command(hist, TEST_CMD_PTR(&cmd3));
    DS_TEST_ASSERT(err == DS_SUCCESS, "execute_command: 3");

    // --- undo ---
    err = ds_history_system_undo(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "undo: 1");
    DS_TEST_ASSERT(ds_history_system_can_undo(hist), "can_undo: after undo");
    DS_TEST_ASSERT(ds_history_system_can_redo(hist), "can_redo: after undo");

    err = ds_history_system_undo(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "undo: 2");

    err = ds_history_system_undo(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "undo: 3");
    DS_TEST_ASSERT(!ds_history_system_can_undo(hist), "can_undo: after all undo");

    // --- redo ---
    err = ds_history_system_redo(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "redo: 1");
    err = ds_history_system_redo(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "redo: 2");
    err = ds_history_system_redo(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "redo: 3");
    DS_TEST_ASSERT(!ds_history_system_can_redo(hist), "can_redo: after all redo");

    // --- clear ---
    err = ds_history_system_clear(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "clear: DS_SUCCESS");
    DS_TEST_ASSERT(!ds_history_system_can_undo(hist), "can_undo: after clear");

    // --- NULL安全 ---
    err = ds_history_system_execute_command(NULL, TEST_CMD_PTR(&cmd1));
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "execute_command: NULL hist");
    err = ds_history_system_execute_command(hist, NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "execute_command: NULL cmd");
    err = ds_history_system_undo(NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "undo: NULL");
    err = ds_history_system_redo(NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "redo: NULL");
    err = ds_history_system_clear(NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "clear: NULL");

    // --- 破棄 ---
    err = ds_history_system_destroy(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
    DS_TEST_ASSERT(ds_history_system_destroy(NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_history_system_basic 完了");
}