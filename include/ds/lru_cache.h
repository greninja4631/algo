<<<<<<< HEAD
#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stddef.h>
=======
#ifndef DS_LRU_CACHE_H
#define DS_LRU_CACHE_H

#include <stddef.h>
#include <stdbool.h>
<<<<<<< HEAD
#include "hashmap.h"
>>>>>>> feature
=======
>>>>>>> feature
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD
<<<<<<< HEAD
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

=======
>>>>>>> feature
// LRUキャッシュの生成・破棄
ds_error_t ds_lru_cache_create(size_t capacity, ds_lru_cache_t** out_cache);
=======
/**
 * @file ds/lru_cache.h
 * @brief LRUキャッシュAPI（Opaque型＋DIアロケータ対応）
 * @details 本体定義は非公開（src/ds/lru_cache.cのみ）
 */
>>>>>>> feature

/**
 * @brief LRUキャッシュ生成
 * @param[in]  alloc     アロケータ
 * @param[in]  capacity  最大要素数
 * @param[out] out_cache 生成されたLRUキャッシュ
 * @return DS_SUCCESS等
 * @ownership caller frees (destroy)
 */
ds_error_t ds_lru_cache_create(
    const ds_allocator_t* alloc,
    size_t capacity,
    ds_lru_cache_t** out_cache
);

/**
 * @brief LRUキャッシュ破棄
 */
ds_error_t ds_lru_cache_destroy(
    const ds_allocator_t* alloc,
    ds_lru_cache_t* cache
);

<<<<<<< HEAD
// キャッシュの要素数
ds_error_t ds_lru_cache_size(const ds_lru_cache_t* cache, size_t* out_size);
>>>>>>> feature
=======
/**
 * @brief 要素追加（key, value）
 */
ds_error_t ds_lru_cache_put(
    const ds_allocator_t* alloc,
    ds_lru_cache_t* cache,
    const char* key,
    void* value
);

/**
 * @brief 要素取得
 */
ds_error_t ds_lru_cache_get(
    const ds_allocator_t* alloc,
    ds_lru_cache_t* cache,
    const char* key,
    void** out_value
);

/**
 * @brief キャッシュの要素数取得
 */
ds_error_t ds_lru_cache_size(
    const ds_allocator_t* alloc,
    const ds_lru_cache_t* cache,
    size_t* out_size
);
>>>>>>> feature

#ifdef __cplusplus
}
#endif

<<<<<<< HEAD
#endif // LRU_CACHE_H
=======
#endif // DS_LRU_CACHE_H
>>>>>>> feature
