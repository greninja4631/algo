#ifndef DS_HASHMAP_H
#define DS_HASHMAP_H

#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {             /* C++ 互換 */
#endif


/* ─ コールバック型は *_func_t で統一 ─ */
typedef void (*ds_hashmap_free_func_t)(void*);

/* ─ API ─ */
ds_error_t ds_hashmap_create(size_t               capacity,
                             ds_hashmap_free_func_t key_free,
                             ds_hashmap_free_func_t val_free,
                             ds_hashmap_t**       out_map);

ds_error_t ds_hashmap_destroy(ds_hashmap_t* map);
ds_error_t ds_hashmap_put    (ds_hashmap_t* map, const char* key, void* value);
ds_error_t ds_hashmap_get    (ds_hashmap_t* map, const char* key, void** out_val);
ds_error_t ds_hashmap_remove (ds_hashmap_t* map, const char* key);
size_t     ds_hashmap_size   (const ds_hashmap_t* map);

#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif /* DS_HASHMAP_H */