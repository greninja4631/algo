#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include "data_structures.h"

// --- キーや値の解放用カスタム関数ポインタ ---
typedef void (*ds_hashmap_free_func)(void*);

#ifdef __cplusplus
extern "C" {
#endif

// --- 不透明型宣言（本体は.cで定義） ---
typedef struct ds_hashmap ds_hashmap_t;

// --- APIプロトタイプ ---
ds_hashmap_t* ds_hashmap_create(size_t capacity, ds_hashmap_free_func key_free, ds_hashmap_free_func val_free);
ds_error_t ds_hashmap_destroy(ds_hashmap_t* map);
ds_error_t ds_hashmap_put(ds_hashmap_t* map, const char* key, void* value);
ds_error_t ds_hashmap_get(ds_hashmap_t* map, const char* key, void** out_value);
ds_error_t ds_hashmap_remove(ds_hashmap_t* map, const char* key);
size_t ds_hashmap_size(const ds_hashmap_t* map);

#ifdef __cplusplus
}
#endif

#endif // HASHMAP_H