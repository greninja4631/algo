#ifndef DS_PROCESS_H
#define DS_PROCESS_H
#include "data_structures.h"     /* ds_allocator_t / ds_error_t */

#ifdef __cplusplus
extern "C" { 
#endif



/* @ownership caller (destroy) */
ds_error_t ds_process_create(
    const ds_allocator_t *alloc,
    int32_t               pid,
    int32_t               burst_time,
    ds_process_t        **out_process);

/* NULL-safe / 冪等 */
ds_error_t ds_process_destroy(
    const ds_allocator_t *alloc,
    ds_process_t         *proc);

int32_t ds_process_get_id        (const ds_process_t *proc);
int32_t ds_process_get_burst_time(const ds_process_t *proc);

#ifdef __cplusplus
}
#endif
#endif /* DS_PROCESS_H */
