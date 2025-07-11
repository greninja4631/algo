#ifndef DS_STACK_H
#define DS_STACK_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif



ds_error_t ds_stack_create(ds_stack_t **out_stack);
ds_error_t ds_stack_destroy(ds_stack_t *stack);
ds_error_t ds_stack_push(ds_stack_t *stack, void *data);
ds_error_t ds_stack_pop(ds_stack_t *stack, void **out_data);
ds_error_t ds_stack_peek(const ds_stack_t *stack, void **out_data);
bool       ds_stack_is_empty(const ds_stack_t *stack);
size_t     ds_stack_size(const ds_stack_t *stack);
ds_error_t ds_stack_reset(ds_stack_t *stack);
ds_error_t ds_stack_get_stats(const ds_stack_t *stack, ds_stats_t *stats);

#ifdef __cplusplus
}
#endif

#endif // DS_STACK_H