#ifndef UTIL_METRICS_H
#define UTIL_METRICS_H

#include <stddef.h>           // size_t
#include <stdint.h>           // int64_t
#include "data_structures.h"  // ds_error_t, ds_allocator_t, ds_metrics_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file    include/util/metrics.h
 * @brief   Metrics（メトリクス/監視カウンタ）API宣言ヘッダ
 * @details
 * - 依存型（ds_error_t, ds_allocator_t, ds_metrics_t）は data_structures.h で前方宣言。
 * - すべての公開APIの第1引数はconst ds_allocator_t *alloc固定。
 * - @ownership は Doxygen コメントで管理。
 */

/** @ownership: caller frees (metricsは呼び出し側で管理) */
ds_error_t ds_metrics_init(const ds_allocator_t *alloc, ds_metrics_t *metrics);

/** @ownership: caller provides metrics, callee does not free */
void ds_metrics_print(const ds_allocator_t *alloc, const ds_metrics_t *metrics);

/** @ownership: no allocation; 全メトリクスをリセット */
void ds_metrics_reset_all(const ds_allocator_t *alloc);

/** @ownership: increment 指定カウンタ名（ヒット数等） */
void ds_metrics_increment(const ds_allocator_t *alloc, const char *name);

/** @ownership: get 指定カウンタ値（int64_t返却） */
int64_t ds_metrics_get(const ds_allocator_t *alloc, const char *name);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_METRICS_H */
