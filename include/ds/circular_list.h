#ifndef DS_CIRCULAR_LIST_H
#define DS_CIRCULAR_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"          /* ds_error_t / ds_stats_t / ds_allocator_t */

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque handle ------------------------------------------------------ */

/* Life-cycle --------------------------------------------------------- */
ds_error_t ds_circular_list_create (const ds_allocator_t *alloc,
                                    ds_circular_list_t  **out_list);
ds_error_t ds_circular_list_destroy(const ds_allocator_t *alloc,
                                    ds_circular_list_t   *list);

/* Mutation ----------------------------------------------------------- */
ds_error_t ds_circular_list_insert (const ds_allocator_t *alloc,
                                    ds_circular_list_t   *list,
                                    void                 *data);
ds_error_t ds_circular_list_remove (const ds_allocator_t *alloc,
                                    ds_circular_list_t   *list,
                                    void                **out_data);

/* Query -------------------------------------------------------------- */
bool        ds_circular_list_is_empty(const ds_circular_list_t *list);
size_t      ds_circular_list_size    (const ds_circular_list_t *list);
ds_error_t  ds_circular_list_get_stats(const ds_circular_list_t *list,
                                       ds_stats_t               *out_stats);

#ifdef __cplusplus
}
#endif
#endif /* DS_CIRCULAR_LIST_H */
