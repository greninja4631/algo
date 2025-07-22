#ifndef UTIL_MEMORY_H
#define UTIL_MEMORY_H

#include <stddef.h>
#include "data_structures.h"  /* ds_allocator_t, ds_error_t だけ */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file    include/util/memory.h
 * @brief   抽象アロケータ経由の安全メモリ確保・解放API（DI/安全fallback対応）
 * @note    公開APIの第1引数は必ずconst ds_allocator_t *alloc
 * @note    alloc==NULL時はDI→標準calloc/freeの三段fallback
 * @note    直malloc/calloc/realloc/free排除（唯一の出口）
 */

/* 標準calloc相当（安全: count==0ならNULL） */
void *ds_malloc (const ds_allocator_t *alloc, size_t count, size_t size);
/* free相当（NULLはno-op） */
void  ds_free   (const ds_allocator_t *alloc, void *ptr);
/* calloc相当（通常はds_mallocと同義実装） */
void *ds_calloc (const ds_allocator_t *alloc, size_t count, size_t size);
/* realloc相当（基本は“新規確保→memcpy→free”。size不明ならmemcpy省略可） */
void *ds_realloc(const ds_allocator_t *alloc, void *ptr, size_t new_size);

/* DI: グローバルmock/allocator注入（テスト用/特殊用途のみ） */
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

#ifdef __cplusplus
}
#endif
#endif /* UTIL_MEMORY_H */