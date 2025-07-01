#ifndef DS_LRU_CACHE_H
#define DS_LRU_CACHE_H

#include <stddef.h>
#include <stdbool.h>
#include "hashmap.h"
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_lru_cache ds_lru_cache_t;

// LRUキャッシュの生成・破棄
ds_error_t ds_lru_cache_create(size_t capacity, ds_lru_cache_t** out_cache);

ds_error_t ds_lru_cache_destroy(ds_lru_cache_t* cache);

// put/get
ds_error_t ds_lru_cache_put(ds_lru_cache_t* cache, const char* key, void* value);
ds_error_t ds_lru_cache_get(ds_lru_cache_t* cache, const char* key, void** out_value);

// キャッシュの要素数
ds_error_t ds_lru_cache_size(const ds_lru_cache_t* cache, size_t* out_size);

#ifdef __cplusplus
}
#endif

#endif // DS_LRU_CACHE_H
