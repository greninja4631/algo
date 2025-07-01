<<<<<<< HEAD
#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stddef.h>
=======
#ifndef DS_LRU_CACHE_H
#define DS_LRU_CACHE_H

#include <stddef.h>
#include <stdbool.h>
#include "hashmap.h"
>>>>>>> feature
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD
// 不透明型宣言
typedef struct ds_lru_cache ds_lru_cache_t;

// --- ユーザー向けAPI ---
ds_lru_cache_t* ds_lru_cache_create(size_t capacity);
ds_error_t ds_lru_cache_destroy(ds_lru_cache_t* cache);
ds_error_t ds_lru_cache_get(ds_lru_cache_t* cache, int key, void** value);
ds_error_t ds_lru_cache_put(ds_lru_cache_t* cache, int key, void* value);
=======
typedef struct ds_lru_cache ds_lru_cache_t;

// LRUキャッシュの生成・破棄
ds_error_t ds_lru_cache_create(size_t capacity, ds_lru_cache_t** out_cache);

ds_error_t ds_lru_cache_destroy(ds_lru_cache_t* cache);

// put/get
ds_error_t ds_lru_cache_put(ds_lru_cache_t* cache, const char* key, void* value);
ds_error_t ds_lru_cache_get(ds_lru_cache_t* cache, const char* key, void** out_value);

// キャッシュの要素数
ds_error_t ds_lru_cache_size(const ds_lru_cache_t* cache, size_t* out_size);
>>>>>>> feature

#ifdef __cplusplus
}
#endif

<<<<<<< HEAD
#endif // LRU_CACHE_H
=======
#endif // DS_LRU_CACHE_H
>>>>>>> feature
