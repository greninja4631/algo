#include <stddef.h>
#include "ds/statistics.h"
#include <stdlib.h>
#include <string.h>
#include <float.h>

struct ds_statistics {
    int   sum, min, max, mode;
    double avg, median;
    size_t n;
    int*   data;  // 配列コピー(必要に応じ)
};

static int cmp_int(const void* a, const void* b) {
    return (*(int*)a) - (*(int*)b);
}

ds_error_t ds_statistics_create(const ds_allocator_t* alloc, ds_statistics_t** out_stats) {
    if (!alloc || !out_stats) return DS_ERR_NULL_POINTER;
    ds_statistics_t* s = alloc->alloc(1, sizeof(*s));
    if (!s) return DS_ERR_ALLOC;
    memset(s, 0, sizeof(*s));
    s->data = NULL;
    *out_stats = s;
    return DS_SUCCESS;
}

void ds_statistics_destroy(const ds_allocator_t* alloc, ds_statistics_t* stats) {
    if (!alloc || !stats) return;
    if (stats->data) { alloc->free(stats->data); stats->data = NULL; }
    alloc->free(stats);
}

ds_error_t ds_statistics_calculate(const ds_allocator_t* alloc, ds_statistics_t* stats, const int* data, size_t size) {
    if (!alloc || !stats || !data || size == 0) return DS_ERR_INVALID_ARG;

    if (stats->data) { alloc->free(stats->data); stats->data = NULL; }
    stats->data = alloc->alloc(size, sizeof(int));
    if (!stats->data) return DS_ERR_ALLOC;
    memcpy(stats->data, data, size * sizeof(int));
    stats->n = size;

    int sum = 0, min = data[0], max = data[0], mode = data[0];
    int freq[1024] = {0};
    for (size_t i = 0; i < size; ++i) {
        sum += data[i];
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
        int idx = data[i] + 512;
        if (idx >= 0 && idx < 1024) freq[idx]++;
    }
    int max_count = 0;
    for (size_t i = 0; i < 1024; ++i) {
        if (freq[i] > max_count) { max_count = freq[i]; mode = (int)i - 512; }
    }
    stats->sum = sum; stats->min = min; stats->max = max; stats->mode = mode;
    stats->avg = (double)sum / size;

    // 中央値計算
    qsort(stats->data, size, sizeof(int), cmp_int);
    if (size % 2 == 0)
        stats->median = (stats->data[size / 2 - 1] + stats->data[size / 2]) / 2.0;
    else
        stats->median = stats->data[size / 2];
    return DS_SUCCESS;
}

#define SAFE_GET(field, out) if (!stats || !(out)) return DS_ERR_NULL_POINTER; *(out) = stats->field; return DS_SUCCESS

ds_error_t ds_statistics_get_sum(const ds_statistics_t* stats, int* out_sum)           { SAFE_GET(sum, out_sum); }
ds_error_t ds_statistics_get_min(const ds_statistics_t* stats, int* out_min)           { SAFE_GET(min, out_min); }
ds_error_t ds_statistics_get_max(const ds_statistics_t* stats, int* out_max)           { SAFE_GET(max, out_max); }
ds_error_t ds_statistics_get_average(const ds_statistics_t* stats, double* out_avg)    { SAFE_GET(avg, out_avg); }
ds_error_t ds_statistics_get_median(const ds_statistics_t* stats, double* out_median)  { SAFE_GET(median, out_median); }
ds_error_t ds_statistics_get_mode(const ds_statistics_t* stats, int* out_mode)         { SAFE_GET(mode, out_mode); }

ds_error_t ds_statistics_calculate_all(
    const ds_allocator_t* alloc,
    const int* data, size_t size,
    int* out_sum, int* out_min, int* out_max,
    double* out_avg, double* out_median, int* out_mode
) {
    ds_statistics_t* s = NULL;
    ds_error_t rc = ds_statistics_create(alloc, &s);
    if (rc != DS_SUCCESS) return rc;
    rc = ds_statistics_calculate(alloc, s, data, size);
    if (rc == DS_SUCCESS) {
        if (out_sum)    *out_sum    = s->sum;
        if (out_min)    *out_min    = s->min;
        if (out_max)    *out_max    = s->max;
        if (out_avg)    *out_avg    = s->avg;
        if (out_median) *out_median = s->median;
        if (out_mode)   *out_mode   = s->mode;
    }
    ds_statistics_destroy(alloc, s);
    return rc;
}