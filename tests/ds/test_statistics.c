#include "ds/statistics.h"
#include "util/memory.h"
#include "util/logger.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Opaque本体: .c限定 */
struct ds_statistics {
    const ds_allocator_t *alloc;
    int   sum, min, max, mode;
    double average, median;
    size_t size;
    int *data;
};

/* 内部ユーティリティ */
static int cmp_int(const void* a, const void* b) {
    return (*(const int*)a) - (*(const int*)b);
}

/* --- 生成・破棄 --- */
ds_error_t ds_statistics_create(const ds_allocator_t *alloc, ds_statistics_t **out_stats) {
    if (!alloc || !out_stats) return DS_ERR_NULL_POINTER;
    ds_statistics_t *s = ds_malloc(alloc, 1, sizeof *s);
    if (!s) return DS_ERR_ALLOC;
    memset(s, 0, sizeof *s);
    s->alloc = alloc;
    *out_stats = s;
    return DS_SUCCESS;
}

void ds_statistics_destroy(const ds_allocator_t *alloc_unused, ds_statistics_t *stats) {
    if (!stats) return;
    if (stats->data) {
        ds_free(stats->alloc, stats->data);
        stats->data = NULL;
    }
    ds_free(stats->alloc ? stats->alloc : alloc_unused, stats);
}

/* --- 計算 --- */
ds_error_t ds_statistics_calculate(
    ds_statistics_t *stats,
    const int *data, size_t n
) {
    if (!stats || !data || n == 0) return DS_ERR_INVALID_ARG;

    if (stats->data) { ds_free(stats->alloc, stats->data); stats->data = NULL; }
    stats->data = ds_malloc(stats->alloc, n, sizeof(int));
    if (!stats->data) return DS_ERR_ALLOC;
    memcpy(stats->data, data, n * sizeof(int));
    stats->size = n;

    int sum = 0, min = data[0], max = data[0], mode = data[0];
    int freq[2048] = {0};
    for (size_t i = 0; i < n; ++i) {
        sum += data[i];
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
        int idx = data[i] + 1024;
        if (idx >= 0 && idx < 2048) freq[idx]++;
    }
    int max_count = 0;
    for (int i = 0; i < 2048; ++i)
        if (freq[i] > max_count) { max_count = freq[i]; mode = i - 1024; }

    stats->sum = sum;
    stats->min = min;
    stats->max = max;
    stats->mode = mode;
    stats->average = (double)sum / n;

    qsort(stats->data, n, sizeof(int), cmp_int);
    stats->median = (n % 2 == 0)
        ? (stats->data[n/2 - 1] + stats->data[n/2]) / 2.0
        : stats->data[n/2];
    return DS_SUCCESS;
}

/* --- ゲッター群 (SAFE_GETはDoxygen表記含む) --- */
#define SAFE_GET(field, out) \
    do { if (!stats || !(out)) return DS_ERR_NULL_POINTER; *(out) = stats->field; return DS_SUCCESS; } while (0)

ds_error_t ds_statistics_get_sum    (const ds_statistics_t *stats, int    *out) { SAFE_GET(sum,     out); }
ds_error_t ds_statistics_get_min    (const ds_statistics_t *stats, int    *out) { SAFE_GET(min,     out); }
ds_error_t ds_statistics_get_max    (const ds_statistics_t *stats, int    *out) { SAFE_GET(max,     out); }
ds_error_t ds_statistics_get_average(const ds_statistics_t *stats, double *out) { SAFE_GET(average, out); }
ds_error_t ds_statistics_get_median (const ds_statistics_t *stats, double *out) { SAFE_GET(median,  out); }
ds_error_t ds_statistics_get_mode   (const ds_statistics_t *stats, int    *out) { SAFE_GET(mode,    out); }

#undef SAFE_GET

/* --- 非保持・一括計算API --- */
ds_error_t ds_statistics_calculate_all(
    const ds_allocator_t *alloc,
    const int *data, size_t n,
    int *out_sum, int *out_min, int *out_max,
    double *out_avg, double *out_med, int *out_mode
) {
    if (!alloc || !data || n == 0) return DS_ERR_INVALID_ARG;
    ds_statistics_t *stats = NULL;
    ds_error_t rc = ds_statistics_create(alloc, &stats);
    if (rc != DS_SUCCESS) return rc;
    rc = ds_statistics_calculate(stats, data, n);
    if (rc == DS_SUCCESS) {
        if (out_sum)    *out_sum    = stats->sum;
        if (out_min)    *out_min    = stats->min;
        if (out_max)    *out_max    = stats->max;
        if (out_avg)    *out_avg    = stats->average;
        if (out_med)    *out_med    = stats->median;
        if (out_mode)   *out_mode   = stats->mode;
    }
    ds_statistics_destroy(alloc, stats);
    return rc;
}