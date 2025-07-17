/**
 * @file test_util.c
 * @brief 汎用ユーティリティ・テストコマンドの実装（テスト専用）
 * @details test_main.c など他テストコードからのみ呼び出される。
 */

#include "test_util.h"      // tests/include/test_util.h
#include "logger.h"         // include/util/logger.h（CFLAGSで-Iincludeによりパス解決）

// 必要な標準ライブラリはここでinclude
#include <stdio.h>
#include <stdlib.h>

// --- テスト用コマンド適用/Undo関数（例） ---

ds_error_t test_command_apply(void *ctx)
{
    test_command_t *cmd = (test_command_t *)ctx;
    ds_log(DS_LOG_LEVEL_INFO,
           "[apply] op=%d value=%d", cmd->op_code, cmd->value);
    return DS_SUCCESS;
}

ds_error_t test_command_undo(void *ctx)
{
    test_command_t *cmd = (test_command_t *)ctx;
    ds_log(DS_LOG_LEVEL_INFO,
           "[undo ] op=%d value=%d", cmd->op_code, cmd->value);
    return DS_SUCCESS;
}
