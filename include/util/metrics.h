#ifndef UTIL_METRICS_H
#define UTIL_METRICS_H

#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Opaque型宣言 */

/** API */
ds_error_t ds_metrics_init(const ds_allocator_t* alloc, ds_metrics_t* metrics);

ds_error_t ds_metrics_increment_ops(const ds_allocator_t* alloc, ds_metrics_t* metrics);
ds_error_t ds_metrics_increment_elements(const ds_allocator_t* alloc, ds_metrics_t* metrics);
ds_error_t ds_metrics_decrement_elements(const ds_allocator_t* alloc, ds_metrics_t* metrics);
ds_error_t ds_metrics_add_memory(const ds_allocator_t* alloc, ds_metrics_t* metrics, size_t bytes);
ds_error_t ds_metrics_sub_memory(const ds_allocator_t* alloc, ds_metrics_t* metrics, size_t bytes);

void ds_metrics_print(const ds_allocator_t* alloc, const ds_metrics_t* metrics);
void ds_metrics_reset_all(const ds_allocator_t* alloc);

void ds_metrics_increment(const ds_allocator_t* alloc, const char* name);
int64_t ds_metrics_get(const ds_allocator_t* alloc, const char* name);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_METRICS_H */
