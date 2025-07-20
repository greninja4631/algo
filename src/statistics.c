/**
 * @file    src/ds/statistics.c
 * @brief   ds_statistics モジュール実装（Opaque + 抽象アロケータ DI完全対応）
 * @details
 *   - コア原則・設計ドキュメントに完全準拠
 *   - 全API const ds_allocator_t* alloc を第1引数
 *   - malloc/calloc/free/realloc 直呼び完全禁止
 *   - NULLセーフ & @ownership Doxygen記載
 *   - destroy系APIで二重free絶対禁止
 *   - static関数/型・DTO以外の内部構造体本体は .cのみに限定
 *   - ログは必ず ds_log() 経由
 */

#include "ds/statistics.h"
#include "util/memory.h"     // ds_malloc / ds_free
#include "util/logger.h"     // ds_log()
#include <stddef.h>
#include <stdlib.h> 
#include <string.h>

/*──────────────────────────────────────
 *  内部型定義（Opaque 本体）
 *──────────────────────────────────────*/
struct ds_statistics {
    const ds_allocator_t *alloc;   ///< @ownership: caller DI or default
    int    *data;                  ///< @ownership: self (destroy時free)
    size_t  size;
    int     sum, min, max, mode;
    double  average, median;
};

/*──────────────────────────────────────
 *  内部ユーティリティ
 *──────────────────────────────────────*/
static inline void *xalloc(const ds_allocator_t *a, size_t n, size_t sz)
{ return ds_malloc(a, n, sz); }

static inline void xfree(const ds_allocator_t *a, void *p)
{ ds_free(a, p); }

static int cmp_int(const void *a, const void *b)
{ return (*(const int *)a) - (*(const int *)b); }

/*──────────────────────────────────────
 *  生成／破棄
 *──────────────────────────────────────*/
/**
 * @brief    統計インスタンス生成
 * @ownership caller frees (destroy)
 */
ds_error_t ds_statistics_create(const ds_allocator_t *alloc,
                                ds_statistics_t     **out_stats)
{
    if (!out_stats) return DS_ERR_NULL_POINTER;

    ds_statistics_t *s = xalloc(alloc, 1, sizeof *s);
    if (!s) return DS_ERR_ALLOC;

    memset(s, 0, sizeof *s);
    s->alloc = alloc;
    *out_stats = s;
    return DS_SUCCESS;
}

/**
 * @brief    統計インスタンス破棄
 * @ownership callee frees
 */
void ds_statistics_destroy(const ds_allocator_t *alloc_unused,
                           ds_statistics_t      *stats)
{
    if (!stats) return;
    xfree(stats->alloc, stats->data);
    xfree(stats->alloc ? stats->alloc : alloc_unused, stats);
}

/*──────────────────────────────────────
 *  コア計算
 *──────────────────────────────────────*/
/**
 * @brief    データを統計インスタンスへ格納＆計算
 */
ds_error_t ds_statistics_calculate(ds_statistics_t *s,
                                   const int       *data,
                                   size_t           n)
{
    if (!s || !data || n == 0) return DS_ERR_INVALID_ARG;

    // 既存バッファ解放 & 再確保
    xfree(s->alloc, s->data);
    s->data = xalloc(s->alloc, n, sizeof(int));
    if (!s->data) return DS_ERR_ALLOC;

    memcpy(s->data, data, n * sizeof(int));
    s->size = n;

    // 基本統計
    int sum = 0, min = data[0], max = data[0];
    for (size_t i = 0; i < n; ++i) {
        sum += data[i];
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
    }
    s->sum = sum;  s->min = min; s->max = max;
    s->average = (double)sum / n;

    // モード（[-1024,1024]範囲の簡易実装）
    int freq[2048] = {0}, mode = data[0], max_cnt = 0;
    for (size_t i = 0; i < n; ++i) {
        int idx = data[i] + 1024;
        if (idx >= 0 && idx < 2048 && ++freq[idx] > max_cnt) {
            max_cnt = freq[idx];
            mode = data[i];
        }
    }
    s->mode = mode;

    // 中央値
    qsort(s->data, n, sizeof(int), cmp_int);
    s->median = (n % 2 == 0)
        ? (s->data[n/2 - 1] + s->data[n/2]) / 2.0
        :  s->data[n/2];

    return DS_SUCCESS;
}

/*──────────────────────────────────────
 *  ゲッター群
 *──────────────────────────────────────*/
#define SAFE_GET(field, out) \
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
/**
 * @brief    生データ配列から統計値を一括計算（インスタンス非保持・便利API）
 * @details  work領域もds_malloc/ds_freeで厳格管理し、malloc/free直呼び禁止
 */
ds_error_t ds_statistics_calculate_all(const ds_allocator_t *alloc,
                                       const int *data, size_t n,
                                       int *out_sum, int *out_min, int *out_max,
                                       double *out_avg, double *out_med, int *out_mode)
{
    if (!data || n == 0) return DS_ERR_INVALID_ARG;

    int *work = xalloc(alloc, n, sizeof(int));   // ← [❌] malloc → ds_malloc
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

    xfree(alloc, work);   // ← [❌] free → ds_free

    if (out_sum)  *out_sum  = s;
    if (out_min)  *out_min  = mi;
    if (out_max)  *out_max  = ma;
    if (out_avg)  *out_avg  = (double)s / n;
    if (out_med)  *out_med  = md;
    if (out_mode) *out_mode = mo;
    return DS_SUCCESS;
}