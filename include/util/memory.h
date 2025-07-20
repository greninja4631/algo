<<<<<<< HEAD
#ifndef MEMORY_H
#define MEMORY_H

=======
#ifndef UTIL_MEMORY_H
#define UTIL_MEMORY_H

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#include "data_structures.h"
>>>>>>> feature
#include <stddef.h>
=======
#include "data_structures.h"  // ds_allocator_t, ds_error_t など
>>>>>>> feature
=======
#include <stddef.h>          // size_t
#include "data_structures.h" // ds_allocator_t, ds_error_t など
>>>>>>> feature
=======
#include <stddef.h>           /* size_t               */
#include "data_structures.h"  /* ds_allocator_t, ds_error_t */
>>>>>>> feature

#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD
<<<<<<< HEAD
// メモリ確保/解放関数型
typedef void* (*ds_malloc_func_t)(size_t size);
typedef void  (*ds_free_func_t)(void* ptr);

// カスタム関数をセット（NULL指定でデフォルトに戻す）
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

// 共通APIとして利用されるds_malloc/ds_free
=======
// アロケータ登録
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);
// ラップ関数
>>>>>>> feature
void* ds_malloc(size_t size);
void  ds_free(void* ptr);
=======
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

<<<<<<< HEAD
//=============================
// メトリクス管理API
//=============================

// 構造体初期化
ds_error_t ds_metrics_init(const ds_allocator_t* alloc, ds_metrics_t* metrics);

// 演算・要素・メモリカウント管理
ds_error_t ds_metrics_increment_ops     (const ds_allocator_t* alloc, ds_metrics_t* metrics);
ds_error_t ds_metrics_increment_elements(const ds_allocator_t* alloc, ds_metrics_t* metrics);
ds_error_t ds_metrics_decrement_elements(const ds_allocator_t* alloc, ds_metrics_t* metrics);
ds_error_t ds_metrics_add_memory        (const ds_allocator_t* alloc, ds_metrics_t* metrics, size_t bytes);
ds_error_t ds_metrics_sub_memory        (const ds_allocator_t* alloc, ds_metrics_t* metrics, size_t bytes);

// 現在値の表示・リセット
void ds_metrics_print    (const ds_allocator_t* alloc, const ds_metrics_t* metrics);
void ds_metrics_reset_all(const ds_allocator_t* alloc);

// 名前付きカウンタ管理（テスト/統計用）
void    ds_metrics_increment(const ds_allocator_t* alloc, const char* name);
int64_t ds_metrics_get      (const ds_allocator_t* alloc, const char* name);
>>>>>>> feature

=======
>>>>>>> feature
#ifdef __cplusplus
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#endif // MEMORY_H
=======
#endif // UTIL_MEMORY_H
>>>>>>> feature
=======
#endif /* UTIL_MEMORY_H */
>>>>>>> feature
