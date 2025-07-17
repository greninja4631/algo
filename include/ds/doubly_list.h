#ifndef DS_DOUBLY_LIST_H
#define DS_DOUBLY_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"   /* ds_error_t, ds_allocator_t, ds_stats_t */

#ifdef __cplusplus
extern "C" {   /* C++ ユーザー向け */
#endif


/* Constructor / Destructor ---------------------------------------------- */
/**
 * @ownership out_list: caller frees via destroy
 */
ds_error_t ds_doubly_list_create (const ds_allocator_t *alloc,
                                  ds_doubly_list_t    **out_list);

ds_error_t ds_doubly_list_destroy(const ds_allocator_t *alloc,
                                  ds_doubly_list_t     *list);

/* Insertion -------------------------------------------------------------- */
ds_error_t ds_doubly_list_insert_front(const ds_allocator_t *alloc,
                                       ds_doubly_list_t     *list,
                                       void                 *data);

ds_error_t ds_doubly_list_insert_back (const ds_allocator_t *alloc,
                                       ds_doubly_list_t     *list,
                                       void                 *data);

ds_error_t ds_doubly_list_insert_at   (const ds_allocator_t *alloc,
                                       ds_doubly_list_t     *list,
                                       size_t                index,
                                       void                 *data);

/* Removal ---------------------------------------------------------------- */
ds_error_t ds_doubly_list_remove_front(const ds_allocator_t *alloc,
                                       ds_doubly_list_t     *list,
                                       void                **out_data);

ds_error_t ds_doubly_list_remove_back (const ds_allocator_t *alloc,
                                       ds_doubly_list_t     *list,
                                       void                **out_data);

ds_error_t ds_doubly_list_remove_at   (const ds_allocator_t *alloc,
                                       ds_doubly_list_t     *list,
                                       size_t                index,
                                       void                **out_data);

/* Access ----------------------------------------------------------------- */
ds_error_t ds_doubly_list_get_at(const ds_doubly_list_t *list,
                                 size_t                  index,
                                 void                  **out_data);

bool        ds_doubly_list_is_empty(const ds_doubly_list_t *list);
size_t      ds_doubly_list_size    (const ds_doubly_list_t *list);
ds_error_t  ds_doubly_list_get_stats(const ds_doubly_list_t *list,
                                     ds_stats_t             *out_stats);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* DS_DOUBLY_LIST_H */
