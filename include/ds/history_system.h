#ifndef DS_HISTORY_SYSTEM_H
#define DS_HISTORY_SYSTEM_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ds/history_system.h
 * @brief Undo/Redo履歴管理API（Opaque型＋抽象アロケータDI）
 * @details 実装は src/ds/history_system.c、構造体本体は非公開
 */

/*─── API ───*/

/**
 * @brief ヒストリーシステムの生成
 * @param[in]  alloc         アロケータ
 * @param[in]  max_history   記録可能な最大履歴数
 * @param[out] out_history   生成されたヒストリーシステム
 * @return DS_SUCCESS等
 * @ownership caller frees (destroy)
 */
ds_error_t ds_history_system_create(
    const ds_allocator_t* alloc,
    size_t max_history,
    ds_history_system_t** out_history);

/**
 * @brief システム破棄
 */
ds_error_t ds_history_system_destroy(
    const ds_allocator_t* alloc,
    ds_history_system_t* history);

/**
 * @brief コマンド実行（履歴追加）
 */
ds_error_t ds_history_system_execute_command(
    const ds_allocator_t* alloc,
    ds_history_system_t* history,
    const ds_command_t* command);

/**
 * @brief Undo操作
 */
ds_error_t ds_history_system_undo(
    const ds_allocator_t* alloc,
    ds_history_system_t* history);

/**
 * @brief Redo操作
 */
ds_error_t ds_history_system_redo(
    const ds_allocator_t* alloc,
    ds_history_system_t* history);

/**
 * @brief Undo可能か
 */
bool ds_history_system_can_undo(
    const ds_history_system_t* history);

/**
 * @brief Redo可能か
 */
bool ds_history_system_can_redo(
    const ds_history_system_t* history);

/**
 * @brief 全履歴消去
 */
ds_error_t ds_history_system_clear(
    const ds_allocator_t* alloc,
    ds_history_system_t* history);

#ifdef __cplusplus
}
#endif

#endif // DS_HISTORY_SYSTEM_H
