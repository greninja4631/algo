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

#include "data_structures.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- 構造体本体は.cファイルに隠蔽 ---
typedef struct ds_metrics ds_metrics_t;

// --- 個別メトリクス構造体API ---
ds_error_t ds_metrics_init(ds_metrics_t *metrics);
ds_error_t ds_metrics_increment_ops(ds_metrics_t *metrics);
ds_error_t ds_metrics_increment_elements(ds_metrics_t *metrics);
ds_error_t ds_metrics_decrement_elements(ds_metrics_t *metrics);
ds_error_t ds_metrics_add_memory(ds_metrics_t *metrics, size_t bytes);
ds_error_t ds_metrics_sub_memory(ds_metrics_t *metrics, size_t bytes);
void       ds_metrics_print(const ds_metrics_t *metrics);
void       ds_metrics_reset_all(void);

// --- 🆕 ネームドカウンタAPI（global/labelベース型） ---
/**
 * @brief ラベル名付きカウンタをインクリメント
 * @param name カウンタ名（例："request", "hit", "miss", "test.counter"）
 */
void    ds_metrics_increment(const char *name);

/**
 * @brief ラベル名付きカウンタの現在値を取得
 * @param name カウンタ名
 * @return 現在値（存在しなければ0）
 */
int64_t ds_metrics_get(const char *name);

#ifdef __cplusplus
}
#endif

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
