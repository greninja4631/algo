#ifndef DOUBLY_LIST_H
#define DOUBLY_LIST_H

#include "../data_structures.h" // ds_error_t, ds_stats_t など共通型

#ifdef __cplusplus
extern "C" {
#endif

// 構造体本体は.c側で定義
typedef struct ds_doubly_list ds_doubly_list_t;

// Doubly Linked List API宣言
ds_doubly_list_t* ds_doubly_list_create(void);
ds_error_t ds_doubly_list_destroy(ds_doubly_list_t* list);

ds_error_t ds_doubly_list_insert_front(ds_doubly_list_t* list, void* data);
ds_error_t ds_doubly_list_insert_back(ds_doubly_list_t* list, void* data);
ds_error_t ds_doubly_list_insert_at(ds_doubly_list_t* list, size_t index, void* data);

ds_error_t ds_doubly_list_remove_front(ds_doubly_list_t* list, void** out_data);
ds_error_t ds_doubly_list_remove_back(ds_doubly_list_t* list, void** out_data);
ds_error_t ds_doubly_list_remove_at(ds_doubly_list_t* list, size_t index, void** out_data);

ds_error_t ds_doubly_list_get_at(const ds_doubly_list_t* list, size_t index, void** out_data);

bool   ds_doubly_list_is_empty(const ds_doubly_list_t* list);
size_t ds_doubly_list_size(const ds_doubly_list_t* list);

ds_error_t ds_doubly_list_get_stats(const ds_doubly_list_t* list, ds_stats_t* stats);

#ifdef __cplusplus
}
#endif

#endif // DOUBLY_LIST_H