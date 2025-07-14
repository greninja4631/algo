<<<<<<< HEAD
<<<<<<< HEAD:tests/test_history_system.c
#include "../include/ds/doubly_list.h"
#include "../include/ds/history_system.h"
=======
#include "data_structures.h"
>>>>>>> feature:tests/ds/test_history_system.c
#include <stdio.h>
#include <assert.h>
=======
/**
 * @file   tests/ds/test_history_system.c
 * @brief  ds_history_system_t の単体テスト
 * @note   tests/test_main.c から呼び出される
 */
<<<<<<< HEAD
>>>>>>> feature


=======
>>>>>>> feature
#include "ds/history_system.h"
#include "ds/test_history_system.h"     /* TEST_CMD_PTR / test_command_init */
#include "util/logger.h"

#define DS_TEST_ASSERT(cond, msg)                                          \
    do {                                                                   \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", (msg)); }      \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",        \
                           (msg), __FILE__, __LINE__); }                   \
    } while (0)

/* ───────────────────────────────────────────────
 * 基本動作
 * ─────────────────────────────────────────────── */
void test__history_system_basic(void)
{
    ds_error_t err;
    ds_history_system_t *hist = ds_history_system_create(3);
    DS_TEST_ASSERT(hist, "create: not NULL");

    ds_command_t cmd1, cmd2, cmd3;
    test_command_init(&cmd1, 1, 10);
    test_command_init(&cmd2, 2, 20);
    test_command_init(&cmd3, 3, 30);

    err = ds_history_system_execute_command(hist, TEST_CMD_PTR(&cmd1));
    DS_TEST_ASSERT(err == DS_SUCCESS, "exec cmd1");
    err = ds_history_system_execute_command(hist, TEST_CMD_PTR(&cmd2));
    DS_TEST_ASSERT(err == DS_SUCCESS, "exec cmd2");
    err = ds_history_system_execute_command(hist, TEST_CMD_PTR(&cmd3));
    DS_TEST_ASSERT(err == DS_SUCCESS, "exec cmd3");

    err = ds_history_system_undo(hist);
<<<<<<< HEAD
    DS_TEST_ASSERT(err == DS_SUCCESS, "undo: 2");

    err = ds_history_system_undo(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "undo: 3");
    DS_TEST_ASSERT(!ds_history_system_can_undo(hist), "can_undo: after all undo");

<<<<<<< HEAD
    printf("=== Doubly List ADT Unit Test Passed ===\n");
    return 0;
}
=======
    // --- redo ---
=======
    DS_TEST_ASSERT(err == DS_SUCCESS, "undo 1");
>>>>>>> feature
    err = ds_history_system_redo(hist);
    DS_TEST_ASSERT(err == DS_SUCCESS, "redo 1");

    ds_history_system_destroy(hist);
    DS_TEST_ASSERT(1, "destroy: no crash");
    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__history_system_basic 完了");
}

<<<<<<< HEAD
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
>>>>>>> feature
=======
/* 必要なら edge-case 用の test__history_system_edge_cases もここに実装 */
>>>>>>> feature
