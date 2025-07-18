<<<<<<< HEAD
#ifndef METRICS_H
#define METRICS_H

#include <stddef.h>
#include <stdint.h>

typedef struct ds_stats {
    size_t total_elements;
    size_t memory_allocated;
    size_t operations_count;
    uint64_t creation_timestamp;
} ds_stats_t;

#endif // METRICS_H
=======
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

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#endif // UTIL_METRICS_H


// metrics.h は
// 「データ構造（スタック、キュー、ハッシュマップ等）」の利用状況やパフォーマンスを数値として記録・集計するための仕組みです。

// 例えば、操作回数や総メモリ使用量、要素数、生成時刻などのメトリクス（指標・統計値）を構造体ds_metrics_tで管理します。

// どんな使い道？

// テスト・デバッグ時に「キューに何個要素があるか」「何回pushされたか」「実際にどのぐらいメモリを使っているか」を一元管理

// サーバーやクラウドアプリで統計情報を可視化

// ライブラリの利用者が「中身の状態を知る」ために使う
>>>>>>> feature
=======
#endif // UTIL_METRICS_H
>>>>>>> feature
=======
#endif /* UTIL_METRICS_H */
>>>>>>> feature
=======
#endif /* UTIL_METRICS_H */
>>>>>>> feature
