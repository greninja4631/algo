#ifndef DS_STACK_H
#define DS_STACK_H

/**
 * @file   ds/stack.h
 * @brief  汎用スタックAPI ― Opaque型＋抽象アロケータDI準拠
 *
 * 本体は src/ds/stack.c のみ。全メモリ操作はalloc経由。エラーはds_error_tで統一。
 * すべて「所有権 @ownership」明記でバグ・リークを防ぐ。
 */

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"   // ds_* 前方宣言 / 汎用型

#ifdef __cplusplus
extern "C" {
#endif

/*──────────────────────────────
 * Opaque型前方宣言（本体は.c）
 *──────────────────────────────*/
// typedef struct ds_stack ds_stack_t;  ← data_structures.h に一元化済み

/*──────────────────────────────
 * Public API
 *──────────────────────────────*/

/**
 * @brief  スタック生成
 * @param  alloc      抽象アロケータ (NULL禁止)
 * @param  out_stack  [out]生成ハンドル
 * @retval DS_SUCCESS           正常
 * @retval DS_ERR_NULL_POINTER  引数NULL
 * @retval DS_ERR_ALLOC         メモリ不足
 * @ownership caller-frees (destroy)
 */
ds_error_t ds_stack_create(
    const ds_allocator_t *alloc,
    ds_stack_t          **out_stack);

/**
 * @brief  スタック破棄（NULLセーフ）
 * @param  alloc  抽象アロケータ (NULL許容)
 * @param  stack  スタックハンドル
 * @retval DS_SUCCESS          正常
 * @retval DS_ERR_NULL_POINTER stack==NULL
 * @ownership library-frees (内部全解放、呼び出し側は以降利用不可)
 */
ds_error_t ds_stack_destroy(
    const ds_allocator_t *alloc,
    ds_stack_t           *stack);

/**
 * @brief  プッシュ
 * @ownership library-takes-ownership (dataはスタックの管理下)
 */
ds_error_t ds_stack_push(
    const ds_allocator_t *alloc,
    ds_stack_t           *stack,
    void                 *data);

/**
 * @brief  ポップ
 * @ownership caller-takes-ownership (返却値は呼び出し側で管理)
 */
ds_error_t ds_stack_pop(
    const ds_allocator_t *alloc,
    ds_stack_t           *stack,
    void                **out_data);

/**
 * @brief  先頭要素を参照（popしない）
 * @ownership borrowed
 */
ds_error_t ds_stack_peek(
    const ds_allocator_t *alloc,
    const ds_stack_t     *stack,
    void                **out_data);

/**
 * @brief 空判定
 */
bool ds_stack_is_empty(const ds_allocator_t *alloc, const ds_stack_t *stack);

/**
 * @brief 要素数
 */
size_t ds_stack_size(const ds_allocator_t *alloc, const ds_stack_t *stack);

/**
 * @brief 全要素クリア
 */
ds_error_t ds_stack_reset(
    const ds_allocator_t *alloc,
    ds_stack_t           *stack);

/**
 * @brief 統計取得
 */
ds_error_t ds_stack_get_stats(
    const ds_allocator_t *alloc,
    const ds_stack_t     *stack,
    ds_stats_t           *out_stats);

/**
 * @brief クローン生成（deep copy, optional 実装例）
 */
ds_error_t ds_stack_clone(
    const ds_allocator_t *alloc,
    const ds_stack_t     *src,
    ds_stack_t          **out_clone);

#ifdef __cplusplus
}
#endif

#endif /* DS_STACK_H */