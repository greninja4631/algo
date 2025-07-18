#ifndef DS_STACK_H
#define DS_STACK_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

/*──────────────────────────────
 * 生成／破棄
 *──────────────────────────────*/
ds_error_t ds_stack_create (const ds_allocator_t *alloc,
                            ds_stack_t          **out_stack);
ds_error_t ds_stack_destroy(const ds_allocator_t *alloc,
                            ds_stack_t           *stack);

/*──────────────────────────────
 * 操作
 *──────────────────────────────*/
ds_error_t ds_stack_push(const ds_allocator_t *alloc,
                         ds_stack_t           *stack,
                         void                 *data);

ds_error_t ds_stack_pop (const ds_allocator_t *alloc,
                         ds_stack_t           *stack,
                         void                **out_data);

ds_error_t ds_stack_peek(const ds_allocator_t *alloc,   /* ←旧コードとの差分 */
                         const ds_stack_t     *stack,
                         void                **out_data);

bool       ds_stack_is_empty(const ds_stack_t *stack);
size_t     ds_stack_size    (const ds_stack_t *stack);

ds_error_t ds_stack_reset(const ds_allocator_t *alloc,
                          ds_stack_t           *stack);

ds_error_t ds_stack_get_stats(const ds_allocator_t *alloc, /* ←alloc を追加 */
                              const ds_stack_t     *stack,
                              ds_stats_t           *out_stats);

#ifdef __cplusplus
}
#endif
#endif /* DS_STACK_H */