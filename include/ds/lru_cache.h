/**
 * @file lru_cache.h
 * @brief LRUキャッシュ（HashMap + Doubly Linked List合成型）
 * 
 * - 責務：最も使われていない要素を自動で削除しつつキャッシュ
 */

 #ifndef LRU_CACHE_H
 #define LRU_CACHE_H
 
 #include <stddef.h>
 #include <stdbool.h>
 #include "hashmap.h"
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 typedef struct lru_cache lru_cache_t;
 
 lru_cache_t* lru_cache_create(size_t capacity);
 void lru_cache_destroy(lru_cache_t* cache);
 
 hashmap_error_t lru_cache_put(lru_cache_t* cache, const char* key, void* value);
 hashmap_error_t lru_cache_get(lru_cache_t* cache, const char* key, void** out_value);
 size_t lru_cache_size(const lru_cache_t* cache);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif // LRU_CACHE_H