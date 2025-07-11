#include "util/metrics.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void ds_metrics_init(ds_metrics_t *metrics) {
    if (!metrics) return;
    memset(metrics, 0, sizeof(ds_metrics_t));
    // creation_timestampは必要に応じてセット
    metrics->creation_timestamp = (uint64_t)time(NULL);
}

void ds_metrics_increment_ops(ds_metrics_t *metrics) {
    if (!metrics) return;
    metrics->operations_count++;
}

void ds_metrics_increment_elements(ds_metrics_t *metrics) {
    if (!metrics) return;
    metrics->total_elements++;
}

void ds_metrics_decrement_elements(ds_metrics_t *metrics) {
    if (!metrics) return;
    if (metrics->total_elements > 0)
        metrics->total_elements--;
}

void ds_metrics_add_memory(ds_metrics_t *metrics, size_t bytes) {
    if (!metrics) return;
    metrics->memory_allocated += bytes;
}

void ds_metrics_sub_memory(ds_metrics_t *metrics, size_t bytes) {
    if (!metrics) return;
    if (metrics->memory_allocated >= bytes)
        metrics->memory_allocated -= bytes;
    else
        metrics->memory_allocated = 0;
}

void ds_metrics_print(const ds_metrics_t *metrics) {
    if (!metrics) return;
    printf("Elements: %zu | Memory: %zu bytes | Operations: %zu | Created: %llu\n",
           metrics->total_elements,
           metrics->memory_allocated,
           metrics->operations_count,
           (unsigned long long)metrics->creation_timestamp);
}
