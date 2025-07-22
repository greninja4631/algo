/**
 * @file    src/util/memory.c
 * @brief   共通メモリアロケータラッパ（DI, セーフティ, Fallback, Mock）
 * @details
 *  - alloc==NULL時は「グローバルDI→標準calloc/free」3段fallback
 *  - count==0/NULL→NULL, free(NULL)はno-op
 *  - 直malloc/calloc/free/realloc完全排除
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"
#include "util/memory.h"
#include "util/logger.h"

/*=========================*
 * グローバルDIモック
 *=========================*/
static ds_malloc_func_t g_custom_malloc = NULL;
static ds_free_func_t   g_custom_free   = NULL;

/*=========================*
 * グローバルDI差し替え
 *=========================*/
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func)
{
    g_custom_malloc = malloc_func;
    g_custom_free   = free_func;
}

static inline void *default_calloc(size_t cnt, size_t sz)
{ return cnt ? calloc(cnt, sz) : NULL; }

/*=========================*
 * 公開API
 *=========================*/
void *ds_malloc(const ds_allocator_t *alloc, size_t cnt, size_t sz)
{
    if (alloc && alloc->alloc) return alloc->alloc(cnt, sz);
    if (g_custom_malloc)       return g_custom_malloc(cnt * sz);
    return default_calloc(cnt, sz);
}

void *ds_calloc(const ds_allocator_t *alloc, size_t cnt, size_t sz)
{ return ds_malloc(alloc, cnt, sz); }

void *ds_realloc(const ds_allocator_t *alloc, void *ptr, size_t new_sz)
{
    if (!ptr) return ds_malloc(alloc, 1, new_sz);
    void *newp = ds_malloc(alloc, 1, new_sz);
    if (!newp) return NULL;
    /* 注意: 呼び出し側が元サイズ把握前提。不明ならmemcpyは省略可 */
    memcpy(newp, ptr, new_sz);
    ds_free(alloc, ptr);
    return newp;
}

void ds_free(const ds_allocator_t *alloc, void *ptr)
{
    if (!ptr) return;
    if (alloc && alloc->free) { alloc->free(ptr); return; }
    if (g_custom_free)        { g_custom_free(ptr); return; }
    free(ptr);
}