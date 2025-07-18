#ifndef UTIL_MEMORY_H
#define UTIL_MEMORY_H

#include <stddef.h>          // size_t
#include "data_structures.h" // ds_allocator_t, ds_error_t など

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file    include/util/memory.h
 * @brief   メモリアロケーションAPI宣言ヘッダ
 * @details
 * - すべて「アロケータalloc経由」に統一。
 * - メモリ確保・解放（malloc/calloc/realloc/free）のみ提供。
 * - メトリクス管理APIは metrics.h で一元管理（ここでは絶対に宣言しない）。
 */

//=============================
// メモリアロケーションAPI
//=============================
void* ds_malloc (const ds_allocator_t* alloc, size_t count, size_t size);
void  ds_free   (const ds_allocator_t* alloc, void* ptr);
void* ds_calloc (const ds_allocator_t* alloc, size_t count, size_t size);
void* ds_realloc(const ds_allocator_t* alloc, void* ptr, size_t new_size);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_MEMORY_H */