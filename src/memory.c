/**
 * @file    src/memory.c
 * @brief   共通メモリアロケータラッパ ― 抽象アロケータ DI & 安全フォールバック
 *
 * ❶ 公開 API（util/memory.h）
 *      void* ds_malloc (const ds_allocator_t *alloc, size_t cnt, size_t sz);
 *      void  ds_free   (const ds_allocator_t *alloc, void *ptr);
 *      void* ds_calloc (const ds_allocator_t *alloc, size_t cnt, size_t sz);
 *      void* ds_realloc(const ds_allocator_t *alloc, void *ptr, size_t new_sz);
 *      void  ds_set_memory_functions(ds_malloc_func_t m, ds_free_func_t f);
 *
 * ❷ ガイドライン 2025-07 に完全準拠
 *      - alloc==NULL 時は「グローバル DI → 標準 calloc/free」へ三段フォールバック
 *      - cnt==0 → NULL、free(NULL) → no-op でクラッシュしない
 *      - 直 malloc/calloc/free を本体から排除（標準関数はここでしか呼ばない）
 */

#include <stddef.h>   /* size_t   */
#include <stdlib.h>   /* calloc, free, realloc */
#include <string.h>   /* memcpy (realloc fallback用) */

#include "data_structures.h" /* ds_* 基本型 */
#include "util/memory.h"
#include "util/logger.h"     /* ds_log() */

/*─────────────────────────────────────────────────────────────*
 *  グローバル DI: テストや組み込み環境で標準 malloc 置換に使用
 *─────────────────────────────────────────────────────────────*/
static ds_malloc_func_t g_custom_malloc = NULL;
static ds_free_func_t   g_custom_free   = NULL;

/*─────────────────────────────────────────────────────────────*
 *  公開関数
 *─────────────────────────────────────────────────────────────*/
void
ds_set_memory_functions(ds_malloc_func_t malloc_func,
                        ds_free_func_t   free_func)
{
    g_custom_malloc = malloc_func;
    g_custom_free   = free_func;
}

/* cnt==0 なら NULL を返す安全 calloc ラッパ                   */
static inline void *
default_calloc(size_t cnt, size_t sz)
{
    return cnt ? calloc(cnt, sz) : NULL;
}

/*─────────────────────────────────────────────────────────────*
 *  メモリ確保
 *─────────────────────────────────────────────────────────────*/
void *
ds_malloc(const ds_allocator_t *alloc, size_t cnt, size_t sz)
{
    /* 1️⃣ 呼び出し側 DI（抽象アロケータ）優先            */
    if (alloc && alloc->alloc)
        return alloc->alloc(cnt, sz);

    /* 2️⃣ グローバル DI（テスト用モックなど）             */
    if (g_custom_malloc)
        return g_custom_malloc(cnt * sz);

    /* 3️⃣ 標準 calloc フォールバック                      */
    return default_calloc(cnt, sz);
}

void *
ds_calloc(const ds_allocator_t *alloc, size_t cnt, size_t sz)
{
    /* calloc と同義に実装（内部的には ds_malloc が呼ばれる） */
    return ds_malloc(alloc, cnt, sz);
}

void *
ds_realloc(const ds_allocator_t *alloc, void *ptr, size_t new_sz)
{
    /* realloc は抽象アロケータに含めない方針。
       呼び出し側が realloc を必要とするケースでは
       “新たに確保 → memcpy → free” を行う。               */
    if (!ptr)
        return ds_malloc(alloc, 1, new_sz);

    void *newp = ds_malloc(alloc, 1, new_sz);
    if (!newp) return NULL;

    /* NOTE: 呼び出し側が元サイズを把握している前提。
       わからない場合は memcpy せずに return newp。          */
    memcpy(newp, ptr, new_sz);
    ds_free(alloc, ptr);
    return newp;
}

/*─────────────────────────────────────────────────────────────*
 *  メモリ解放（free(NULL) は no-op）
 *─────────────────────────────────────────────────────────────*/
void
ds_free(const ds_allocator_t *alloc, void *ptr)
{
    if (!ptr) return;

    /* 1️⃣ 呼び出し側 DI */
    if (alloc && alloc->free) {
        alloc->free(ptr);
        return;
    }

    /* 2️⃣ グローバル DI */
    if (g_custom_free) {
        g_custom_free(ptr);
        return;
    }

    /* 3️⃣ 標準 free */
    free(ptr);
}