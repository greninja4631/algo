#ifndef DS_LRU_CACHE_H
#define DS_LRU_CACHE_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ds/lru_cache.h
 * @brief LRUキャッシュAPI（Opaque型＋DIアロケータ対応）
 * @details 本体定義は非公開（src/ds/lru_cache.cのみ）
 */

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

#ifdef __cplusplus
}
#endif

#endif // DS_LRU_CACHE_H
