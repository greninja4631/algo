#ifndef UTIL_MEMORY_H
#define UTIL_MEMORY_H

#include "data_structures.h"  // ds_allocator_t, ds_error_t など

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file    util/memory.h
 * @brief   メモリアロケーションAPI + メトリクス管理API
 * @details すべて「アロケータalloc経由」に統一。メモリ確保・解放・メトリクス管理を提供。
 */

//=============================
// メモリアロケーションAPI
//=============================
void* ds_malloc (const ds_allocator_t* alloc, size_t count, size_t size);
void  ds_free   (const ds_allocator_t* alloc, void* ptr);
void* ds_calloc (const ds_allocator_t* alloc, size_t count, size_t size);
void* ds_realloc(const ds_allocator_t* alloc, void* ptr, size_t new_size);

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

#ifdef __cplusplus
}
#endif

#endif /* UTIL_MEMORY_H */