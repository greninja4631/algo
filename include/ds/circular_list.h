#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"    // ← これを追加！

typedef struct ds_circular_list ds_circular_list_t;

ds_circular_list_t* ds_circular_list_create(void);
ds_error_t ds_circular_list_destroy(ds_circular_list_t* list);
ds_error_t ds_circular_list_insert(ds_circular_list_t* list, void* data);
ds_error_t ds_circular_list_remove(ds_circular_list_t* list, void** data);
bool       ds_circular_list_is_empty(const ds_circular_list_t* list);
size_t     ds_circular_list_size(const ds_circular_list_t* list);

#endif // CIRCULAR_LIST_H