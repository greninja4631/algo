#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

// 不透明型宣言
typedef struct ds_lru_cache ds_lru_cache_t;

// --- ユーザー向けAPI ---
ds_lru_cache_t* ds_lru_cache_create(size_t capacity);
ds_error_t ds_lru_cache_destroy(ds_lru_cache_t* cache);
ds_error_t ds_lru_cache_get(ds_lru_cache_t* cache, int key, void** value);
ds_error_t ds_lru_cache_put(ds_lru_cache_t* cache, int key, void* value);

#ifdef __cplusplus
}
#endif

#endif // LRU_CACHE_H