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

// 構造体：操作回数・メモリ使用量などの集計用
typedef struct {
    size_t   total_elements;       // 現在の要素数
    size_t   memory_allocated;     // 使用メモリ（バイト）
    size_t   operations_count;     // 操作回数
    uint64_t creation_timestamp;   // 作成時刻（任意・未実装なら0でOK）
} ds_metrics_t;

// 操作カウンタを初期化
void ds_metrics_init(ds_metrics_t *metrics);

// 要素追加・削除などで操作数/要素数/メモリサイズを更新
void ds_metrics_increment_ops(ds_metrics_t *metrics);
void ds_metrics_increment_elements(ds_metrics_t *metrics);
void ds_metrics_decrement_elements(ds_metrics_t *metrics);

// メモリ使用量を手動で加算・減算
void ds_metrics_add_memory(ds_metrics_t *metrics, size_t bytes);
void ds_metrics_sub_memory(ds_metrics_t *metrics, size_t bytes);

// 必要に応じてメトリクス情報を表示
void ds_metrics_print(const ds_metrics_t *metrics);

#ifdef __cplusplus
}
#endif

#endif // UTIL_METRICS_H


// metrics.h は
// 「データ構造（スタック、キュー、ハッシュマップ等）」の利用状況やパフォーマンスを数値として記録・集計するための仕組みです。

// 例えば、操作回数や総メモリ使用量、要素数、生成時刻などのメトリクス（指標・統計値）を構造体ds_metrics_tで管理します。

// どんな使い道？

// テスト・デバッグ時に「キューに何個要素があるか」「何回pushされたか」「実際にどのぐらいメモリを使っているか」を一元管理

// サーバーやクラウドアプリで統計情報を可視化

// ライブラリの利用者が「中身の状態を知る」ために使う
>>>>>>> feature
