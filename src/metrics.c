/**
 * @file    src/metrics.c
 * @brief   軽量メトリクス収集・管理モジュール
 * @details
 *   - Opaque本体は.cに隠蔽
 *   - すべての関数がガイドライン形式（DI/シグネチャ/エラー処理統一）
 *   - allocator DI／alloc==NULL時は標準calloc/free
 *   - printf禁止→ds_log()のみ
 *   - -Werror下で未使用警告ゼロ
 */

#include "data_structures.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "util/metrics.h"
#include "util/logger.h"

//---------------------------------------------------
// Opaque 本体
//---------------------------------------------------
struct ds_metrics {
    size_t   total_elements;
    size_t   memory_allocated;
    size_t   operations_count;
    uint64_t creation_timestamp;
};

//---------------------------------------------------
// DIアロケータラッパ
//---------------------------------------------------
#define ALLOC(a, n, sz) ((a) ? (a)->alloc((n), (sz)) : calloc((n), (sz)))
#define FREE(a, p)      do { if ((p)) { (a) ? (a)->free(p) : free(p); } } while (0)

//---------------------------------------------------
// 基本API
//---------------------------------------------------
ds_error_t
ds_metrics_init(const ds_allocator_t *alloc, ds_metrics_t *metrics)
{
    (void)alloc;
    if (!metrics) return DS_ERR_NULL_POINTER;
    memset(metrics, 0, sizeof *metrics);
    metrics->creation_timestamp = (uint64_t)time(NULL);
    return DS_SUCCESS;
}

ds_error_t
ds_metrics_increment_ops(const ds_allocator_t *alloc, ds_metrics_t *metrics)
{
    (void)alloc;
    if (!metrics) return DS_ERR_NULL_POINTER;
    metrics->operations_count++;
    return DS_SUCCESS;
}

ds_error_t
ds_metrics_increment_elements(const ds_allocator_t *alloc, ds_metrics_t *metrics)
{
    (void)alloc;
    if (!metrics) return DS_ERR_NULL_POINTER;
    metrics->total_elements++;
    return DS_SUCCESS;
}

ds_error_t
ds_metrics_decrement_elements(const ds_allocator_t *alloc, ds_metrics_t *metrics)
{
    (void)alloc;
    if (!metrics) return DS_ERR_NULL_POINTER;
    if (metrics->total_elements) metrics->total_elements--;
    return DS_SUCCESS;
}

ds_error_t
ds_metrics_add_memory(const ds_allocator_t *alloc, ds_metrics_t *metrics, size_t bytes)
{
    (void)alloc;
    if (!metrics) return DS_ERR_NULL_POINTER;
    metrics->memory_allocated += bytes;
    return DS_SUCCESS;
}

ds_error_t
ds_metrics_sub_memory(const ds_allocator_t *alloc, ds_metrics_t *metrics, size_t bytes)
{
    (void)alloc;
    if (!metrics) return DS_ERR_NULL_POINTER;
    if (metrics->memory_allocated >= bytes)
        metrics->memory_allocated -= bytes;
    else
        metrics->memory_allocated = 0;
    return DS_SUCCESS;
}

//---------------------------------------------------
// ログ出力API
//---------------------------------------------------
void
ds_metrics_print(const ds_allocator_t *alloc, const ds_metrics_t *m)
{
    (void)alloc;
    if (!m) {
        ds_log(DS_LOG_LEVEL_INFO, "[metrics] (null)");
        return;
    }
    ds_log(DS_LOG_LEVEL_INFO,
        "[metrics] elements=%zu  mem=%zuB  ops=%zu  created=%llu",
        m->total_elements,
        m->memory_allocated,
        m->operations_count,
        (unsigned long long)m->creation_timestamp);
}

//---------------------------------------------------
// 名前付きカウンタ（シンプル実装）
//---------------------------------------------------
typedef struct named_counter {
    char                    name[64];
    int64_t                 value;
    struct named_counter   *next;
} named_counter_t;

static named_counter_t *g_named = NULL;

static named_counter_t *
find_counter(const char *name)
{
    for (named_counter_t *p = g_named; p; p = p->next)
        if (strncmp(p->name, name, sizeof p->name) == 0)
            return p;
    return NULL;
}

void
ds_metrics_increment(const ds_allocator_t *alloc, const char *name)
{
    if (!name) return;
    named_counter_t *c = find_counter(name);
    if (!c) {
        c = ALLOC(alloc, 1, sizeof *c);
        if (!c) return;
        strncpy(c->name, name, sizeof c->name - 1);
        c->name[sizeof c->name - 1] = '\0';
        c->value = 0;
        c->next  = g_named;
        g_named  = c;
    }
    c->value++;
}

int64_t
ds_metrics_get(const ds_allocator_t *alloc, const char *name)
{
    (void)alloc;
    named_counter_t *c = name ? find_counter(name) : NULL;
    return c ? c->value : 0;
}

void
ds_metrics_reset_all(const ds_allocator_t *alloc)
{
    named_counter_t *p = g_named;
    while (p) {
        named_counter_t *n = p->next;
        FREE(alloc, p);
        p = n;
    }
    g_named = NULL;
}