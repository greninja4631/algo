#ifndef DS_CIRCULAR_LIST_H
#define DS_CIRCULAR_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"  // ds_error_t型


// API宣言のみ
ds_error_t ds_circular_list_create(ds_circular_list_t** out_list);
ds_error_t ds_circular_list_destroy(ds_circular_list_t* list);
ds_error_t ds_circular_list_insert(ds_circular_list_t* list, void* data);
ds_error_t ds_circular_list_remove(ds_circular_list_t* list, void** out_data);
bool       ds_circular_list_is_empty(const ds_circular_list_t* list);
size_t     ds_circular_list_size(const ds_circular_list_t* list);

#endif