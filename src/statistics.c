#include <stddef.h>
/**
 * @file    src/ds/statistics.c
 * @brief   ds_statistics モジュール実装（Opaque + 抽象アロケータ DI）
 */
#include "ds/statistics.h"
#include <stdlib.h>
#include <string.h>

/*──────────────────────────────────────
 *  内部型（Opaque 本体）
 *──────────────────────────────────────*/
struct ds_statistics {
    const ds_allocator_t *alloc;   /* ← 呼び出し側 DI アロケータを保持 */
    int    *data;
    size_t  size;
    int     sum, min, max, mode;
    double  average, median;
};

/*──────────────────────────────────────
 *  ユーティリティ
 *──────────────────────────────────────*/
static inline void *xalloc(const ds_allocator_t *a, size_t n, size_t sz)
{ return a ? a->alloc(n, sz) : calloc(n, sz); }

static inline void xfree(const ds_allocator_t *a, void *p)
{ if (p) (a ? a->free(p) : free(p)); }

static int cmp_int(const void *a, const void *b)
{ return (*(const int *)a) - (*(const int *)b); }

/*──────────────────────────────────────
 *  生成／破棄
 *──────────────────────────────────────*/
ds_error_t ds_statistics_create(const ds_allocator_t *alloc,
                                ds_statistics_t     **out_stats)
{
    if (!out_stats) return DS_ERR_NULL_POINTER;

    ds_statistics_t *s = xalloc(alloc, 1, sizeof *s);
    if (!s) return DS_ERR_ALLOC;

    memset(s, 0, sizeof *s);
    s->alloc = alloc;          /* ★ アロケータを保存 */
    *out_stats = s;
    return DS_SUCCESS;
}

void ds_statistics_destroy(const ds_allocator_t *alloc_unused,
                           ds_statistics_t      *stats)
{
    if (!stats) return;
    xfree(stats->alloc, stats->data);
    /* self の解放は self->alloc を優先、無ければ呼び出し引数 */
    xfree(stats->alloc ? stats->alloc : alloc_unused, stats);
}

/*──────────────────────────────────────
 *  コア計算
 *──────────────────────────────────────*/
ds_error_t ds_statistics_calculate(ds_statistics_t *s,
                                   const int       *data,
                                   size_t           n)
{
    if (!s || !data || n == 0) return DS_ERR_INVALID_ARG;

    /* 既存バッファ解放 & 再確保 */
    xfree(s->alloc, s->data);
    s->data = xalloc(s->alloc, n, sizeof(int));
    if (!s->data) return DS_ERR_ALLOC;

    memcpy(s->data, data, n * sizeof(int));
    s->size = n;

    /* 基本統計 */
    int sum = 0, min = data[0], max = data[0];
    for (size_t i = 0; i < n; ++i) {
        sum += data[i];
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
    }
    s->sum = sum;  s->min = min; s->max = max;
    s->average = (double)sum / n;

    /* モード（[-1024,1024] 範囲簡易実装） */
    int freq[2048] = {0}, mode = data[0], max_cnt = 0;
    for (size_t i = 0; i < n; ++i) {
        int idx = data[i] + 1024;
        if (idx >= 0 && idx < 2048 && ++freq[idx] > max_cnt) {
            max_cnt = freq[idx];
            mode = data[i];
        }
    }
    s->mode = mode;

    /* 中央値 */
    qsort(s->data, n, sizeof(int), cmp_int);
    s->median = (n % 2 == 0)
        ? (s->data[n/2 - 1] + s->data[n/2]) / 2.0
        :  s->data[n/2];

    return DS_SUCCESS;
}

/*──────────────────────────────────────
 *  ゲッター
 *──────────────────────────────────────*/
#define SAFE_GET(field, out)                       \
    do { if (!stats || !(out)) return DS_ERR_NULL_POINTER; \
         *(out) = stats->field; return DS_SUCCESS; } while (0)

ds_error_t ds_statistics_get_sum    (const ds_statistics_t *stats, int    *out){ SAFE_GET(sum,     out); }
ds_error_t ds_statistics_get_min    (const ds_statistics_t *stats, int    *out){ SAFE_GET(min,     out); }
ds_error_t ds_statistics_get_max    (const ds_statistics_t *stats, int    *out){ SAFE_GET(max,     out); }
ds_error_t ds_statistics_get_average(const ds_statistics_t *stats, double *out){ SAFE_GET(average, out); }
ds_error_t ds_statistics_get_median (const ds_statistics_t *stats, double *out){ SAFE_GET(median,  out); }
ds_error_t ds_statistics_get_mode   (const ds_statistics_t *stats, int    *out){ SAFE_GET(mode,    out); }

#undef SAFE_GET

/*──────────────────────────────────────
 *  非保持・一括計算ユーティリティ
 *──────────────────────────────────────*/
ds_error_t ds_statistics_calculate_all(const int *data, size_t n,
                                       int *sum, int *min, int *max,
                                       double *avg, double *med, int *mode)
{
    if (!data || n == 0) return DS_ERR_INVALID_ARG;

    /* ワーク領域はスタック / smalloc でも可。ここでは malloc + free */
    int *work = malloc(n * sizeof(int));
    if (!work) return DS_ERR_ALLOC;
    memcpy(work, data, n * sizeof(int));

    int s = 0, mi = data[0], ma = data[0];
    for (size_t i = 0; i < n; ++i) {
        s += data[i];
        if (data[i] < mi) mi = data[i];
        if (data[i] > ma) ma = data[i];
    }

    int freq[2048] = {0}, mo = data[0], mc = 0;
    for (size_t i = 0; i < n; ++i) {
        int idx = data[i] + 1024;
        if (idx >= 0 && idx < 2048 && ++freq[idx] > mc) {
            mc = freq[idx]; mo = data[i];
        }
    }

    qsort(work, n, sizeof(int), cmp_int);
    double md = (n % 2 == 0)
        ? (work[n/2 - 1] + work[n/2]) / 2.0
        :  work[n/2];
    free(work);

    if (sum) *sum = s;
    if (min) *min = mi;
    if (max) *max = ma;
    if (avg) *avg = (double)s / n;
    if (med) *med = md;
    if (mode) *mode = mo;
    return DS_SUCCESS;
}