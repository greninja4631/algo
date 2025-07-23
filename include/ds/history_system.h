#ifndef DS_HISTORY_SYSTEM_H
#define DS_HISTORY_SYSTEM_H
/**
 * @file    history_system.h
 * @brief   Undo/Redo 履歴管理 API（Opaque 型 + 抽象アロケータ DI）
 * @version 1.0.0
 *
 * ● 仕様要点 ──────────────────────────────────────────────
 *   • 構造体本体は src/ds/history_system.c に隠蔽（ABI 安定化）
 *   • すべての API が ds_error_t を返す（ガイドライン §2.1）
 *   • アロケータ DI：alloc==NULL ならデフォルト malloc/free
 *   • 出力引数は out_ プレフィクスで統一
 *   • @ownership タグで所有権を明示（ドキュメント/CI 用）
 *   • -Wall -Wextra -Werror -pedantic で警告ゼロを保証
 *────────────────────────────────────────────────────────*/
#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"   /* 共通型・ds_error_t・ds_allocator_t・ds_command_t */

#ifdef __cplusplus
extern "C" {
#endif


/*======================================================================
 *  Creation / Destruction
 *====================================================================*/
/**
 * @brief   ヒストリーシステムの生成
 * @param[in]  alloc       抽象アロケータ（NULL = デフォルト）
 * @param[in]  max_history 記録可能な最大履歴数（0 で無制限）
 * @param[out] out_history 生成されたハンドル
 * @retval  DS_SUCCESS            正常終了
 * @retval  DS_ERR_NULL_POINTER   引数 NULL
 * @retval  DS_ERR_ALLOC          メモリ確保失敗
 * @ownership caller frees via ::ds_history_system_destroy
 */
ds_error_t ds_history_system_create(
    const ds_allocator_t *alloc,
    size_t                max_history,
    ds_history_system_t **out_history);

/**
 * @brief   ヒストリーシステムの破棄
 * @ownership callee frees
 */
ds_error_t ds_history_system_destroy(
    const ds_allocator_t *alloc,
    ds_history_system_t  *history);

/*======================================================================
 *  Core operations
 *====================================================================*/
/**
 * @brief   コマンド実行（履歴追加）
 * @details command->execute() が成功したときのみ履歴に積まれる。
 */
ds_error_t ds_history_system_execute_command(
    const ds_allocator_t *alloc,
    ds_history_system_t  *history,
    const ds_command_t   *command);

/**
 * @brief Undo 操作
 */
ds_error_t ds_history_system_undo(
    const ds_allocator_t *alloc,
    ds_history_system_t  *history);

/**
 * @brief Redo 操作
 */
ds_error_t ds_history_system_redo(
    const ds_allocator_t *alloc,
    ds_history_system_t  *history);

/*======================================================================
 *  Query helpers
 *====================================================================*/
/** @return Undo 可能なら true */
bool ds_history_system_can_undo(const ds_history_system_t *history);

/** @return Redo 可能なら true */
bool ds_history_system_can_redo(const ds_history_system_t *history);

/**
 * @brief 履歴の全消去（Undo/Redo 両スタックをクリア）
 */
ds_error_t ds_history_system_clear(
    const ds_allocator_t *alloc,
    ds_history_system_t  *history);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* DS_HISTORY_SYSTEM_H */
