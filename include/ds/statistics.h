#ifndef DS_STATISTICS_H
#define DS_STATISTICS_H

#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

ds_error_t ds_statistics_create(const ds_allocator_t* alloc, ds_statistics_t** out_stats);
void       ds_statistics_destroy(const ds_allocator_t* alloc, ds_statistics_t* stats);
ds_error_t ds_statistics_calculate(ds_statistics_t* stats, const int* data, size_t size);

ds_error_t ds_statistics_get_sum    (const ds_statistics_t* stats, int* out_sum);
ds_error_t ds_statistics_get_min    (const ds_statistics_t* stats, int* out_min);
ds_error_t ds_statistics_get_max    (const ds_statistics_t* stats, int* out_max);
ds_error_t ds_statistics_get_average(const ds_statistics_t* stats, double* out_avg);
ds_error_t ds_statistics_get_median (const ds_statistics_t* stats, double* out_median);
ds_error_t ds_statistics_get_mode   (const ds_statistics_t* stats, int* out_mode);

#ifdef __cplusplus
}
#endif

#endif // DS_STATISTICS_H
