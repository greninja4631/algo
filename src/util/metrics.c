// src/util/metrics.c

#include "../../include/data_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

// メトリクス用構造体例（必要に応じて拡張）
typedef struct ds_metrics {
    uint64_t num_alloc;
    uint64_t num_free;
    uint64_t total_bytes_allocated;
    uint64_t total_bytes_freed;
    time_t   created_at;
    // 必要に応じて追加
} ds_metrics_t;

// シングルトン・グローバルメトリクス（本番ではスレッド対応/外部注入も可能）
static ds_metrics_t global_metrics = {0};

// ---- API ----

// メトリクス初期化
void ds_metrics_init(void) {
    global_metrics.num_alloc = 0;
    global_metrics.num_free = 0;
    global_metrics.total_bytes_allocated = 0;
    global_metrics.total_bytes_freed = 0;
    global_metrics.created_at = time(NULL);
}

// メトリクス更新API（メモリ確保・解放ごとに呼び出し想定）
void ds_metrics_update(ssize_t alloc_delta, ssize_t free_delta, ssize_t bytes_delta) {
    if (alloc_delta > 0) global_metrics.num_alloc += alloc_delta;
    if (free_delta > 0) global_metrics.num_free += free_delta;
    if (bytes_delta > 0) global_metrics.total_bytes_allocated += bytes_delta;
    if (bytes_delta < 0) global_metrics.total_bytes_freed += -bytes_delta;
}

// メトリクス取得
const ds_metrics_t* ds_metrics_get(void) {
    return &global_metrics;
}

// デバッグ用: メトリクス出力
void ds_metrics_print(void) {
    printf("[Metrics] alloc: %llu, free: %llu, alloc_bytes: %llu, free_bytes: %llu, since: %lld\n",
        (unsigned long long)global_metrics.num_alloc,
        (unsigned long long)global_metrics.num_free,
        (unsigned long long)global_metrics.total_bytes_allocated,
        (unsigned long long)global_metrics.total_bytes_freed,
        (long long)global_metrics.created_at
    );
}
