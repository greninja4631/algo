#ifndef UTIL_MEMORY_H
#define UTIL_MEMORY_H

#include <stddef.h>           /* size_t               */
#include "data_structures.h"  /* ds_allocator_t, ds_error_t */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file    include/util/memory.h
 * @brief   抽象アロケータ経由のメモリ確保・解放API
 * @details
 *  - すべて公開APIの第1引数にconst ds_allocator_t *allocを取る。
 *  - alloc==NULL時は内部でcalloc/free/reallocへフォールバック。
 *  - 直接malloc/calloc/realloc/freeは一切使用せず、全体のメモリ出口を集約。
 */

/**
 * @brief   メモリ割り当て
 * @pre     count > 0, size > 0
 * @param   alloc 抽象アロケータ（NULL→callocフォールバック）
 * @param   count 要素数
 * @param   size  各要素のバイト数
 * @return  確保されたメモリへのポインタ（NULLならDS_ERR_ALLOC扱い）
 * @ownership caller frees via ds_free/alloc->free
 */
void *ds_malloc(const ds_allocator_t *alloc,
                size_t                count,
                size_t                size);

/**
 * @brief   メモリ解放
 * @pre     ptr != NULL
 * @param   alloc 抽象アロケータ（NULL→freeフォールバック）
 * @param   ptr   解放対象ポインタ
 */
void ds_free(const ds_allocator_t *alloc,
             void                 *ptr);

/**
 * @brief   ゼロ初期化付きメモリ割り当て
 * @pre     count > 0, size > 0
 * @param   alloc 抽象アロケータ（NULL→callocフォールバック）
 * @param   count 要素数
 * @param   size  各要素のバイト数
 * @return  確保されたゼロ初期化メモリへのポインタ
 * @ownership caller frees via ds_free/alloc->free
 */
void *ds_calloc(const ds_allocator_t *alloc,
                size_t                count,
                size_t                size);

/**
 * @brief   メモリ再確保
 * @pre     ptr != NULL, new_size > 0
 * @param   alloc    抽象アロケータ（NULL→malloc+freeフォールバック）
 * @param   ptr      再確保対象ポインタ
 * @param   new_size 新しいバイト数
 * @return  再確保後のポインタ（NULLなら元メモリは解放せず）
 * @ownership caller frees via ds_free/alloc->free
 */
void *ds_realloc(const ds_allocator_t *alloc,
                 void                 *ptr,
                 size_t                new_size);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_MEMORY_H */