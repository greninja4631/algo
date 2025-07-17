#ifndef DS_STACK_H
#define DS_STACK_H

/**
 * @file   ds/stack.h
 * @brief  汎用スタック API ― Opaque 型 + 抽象アロケータ DI 対応
 *
 * – 構造体本体は src/ds/stack.c に隠蔽。
 * – すべてのメモリ操作は DI された @p alloc  経由。
 * – 失敗は ds_error_t で明示。成功時は DS_SUCCESS。
 * – 所有権／副作用を Doxygen タグでドキュメント化 (@ownership)。
 */

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"   /* ds_* 前方宣言 / 汎用型 */

#ifdef __cplusplus
extern "C" {
#endif

/*──────────────────────────────
 * Public API
 *──────────────────────────────*/

/**
 * @brief  スタック生成
 * @param  alloc      抽象アロケータ (NULL 禁止)
 * @param  out_stack  生成されたハンドル
 * @retval DS_SUCCESS           正常
 * @retval DS_ERR_NULL_POINTER  引数 NULL
 * @retval DS_ERR_ALLOC         メモリ不足
 * @ownership caller frees via ds_stack_destroy()
 */
ds_error_t ds_stack_create(const ds_allocator_t *alloc,
                           ds_stack_t          **out_stack);

/**
 * @brief  スタック破棄 (NULL セーフ)
 * @param  alloc  抽象アロケータ (NULL 許容)
 * @param  stack  スタック ハンドル
 * @retval DS_SUCCESS          正常
 * @retval DS_ERR_NULL_POINTER stack == NULL
 * @ownership library frees internals, caller relinquishes ownership
 */
ds_error_t ds_stack_destroy(const ds_allocator_t *alloc,
                            ds_stack_t           *stack);

/** push / pop */
ds_error_t ds_stack_push(const ds_allocator_t *alloc,
                         ds_stack_t           *stack,
                         void                 *data);

ds_error_t ds_stack_pop (const ds_allocator_t *alloc,
                         ds_stack_t           *stack,
                         void                **out_data);

/** peek (no pop) */
ds_error_t ds_stack_peek(const ds_allocator_t *alloc,
                         const ds_stack_t     *stack,
                         void                **out_data);

/* convenience */
bool       ds_stack_is_empty(const ds_stack_t *stack);
size_t     ds_stack_size    (const ds_stack_t *stack);

/** reset  (& stats) */
ds_error_t ds_stack_reset    (const ds_allocator_t *alloc,
                              ds_stack_t           *stack);

ds_error_t ds_stack_get_stats(const ds_allocator_t *alloc,
                              const ds_stack_t     *stack,
                              ds_stats_t           *out_stats);

#ifdef __cplusplus
}
#endif

#endif /* DS_STACK_H */
