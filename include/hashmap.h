#ifndef DS_HASHMAP_H
#define DS_HASHMAP_H
#include <stddef.h>
#include "../data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_hashmap ds_hashmap_t;
typedef void (*ds_hashmap_free_func)(void*);

ds_hashmap_t* ds_hashmap_create(size_t capacity, ds_hashmap_free_func key_free, ds_hashmap_free_func val_free);
ds_error_t ds_hashmap_destroy(ds_hashmap_t* map);
ds_error_t ds_hashmap_put(ds_hashmap_t* map, const char* key, void* value);
ds_error_t ds_hashmap_get(ds_hashmap_t* map, const char* key, void** out_value);
ds_error_t ds_hashmap_remove(ds_hashmap_t* map, const char* key);
size_t ds_hashmap_size(const ds_hashmap_t* map);

#ifdef __cplusplus
}
#endif
#endif // DS_HASHMAP_H