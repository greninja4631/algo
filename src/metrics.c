// src/metrics.c
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include "util/metrics.h"

// --- 内部プロトタイプ ---
static void static_clear_named_metrics(void);

// --- 構造体実体 ---
struct ds_metrics {
    size_t   total_elements;
    size_t   memory_allocated;
    size_t   operations_count;
    uint64_t creation_timestamp;
};

// --- 個別メトリクス管理API ---
ds_error_t ds_metrics_init(ds_metrics_t *metrics) {
    if (!metrics) return DS_ERR_NULL_POINTER;
    memset(metrics, 0, sizeof(ds_metrics_t));
    metrics->creation_timestamp = (uint64_t)time(NULL);
    return DS_SUCCESS;
}

// 以下略 … ds_metrics_increment_ops／increment_elements／decrement_elements／add_memory／sub_memory …

void ds_metrics_print(const ds_metrics_t *metrics) {
    if (!metrics) {
        printf("metrics: (null)\n");
        return;
    }
    printf("Elements: %zu | Memory: %zu bytes | Operations: %zu | Created: %llu\n",
           metrics->total_elements,
           metrics->memory_allocated,
           metrics->operations_count,
           (unsigned long long)metrics->creation_timestamp);
}

/* 修正: ヘッダが void ds_metrics_reset_all(void) なので戻り値は void */
void ds_metrics_reset_all(void) {
    static_clear_named_metrics();
}

// --- 🆕 ネームドカウンタ管理（超軽量・スレッド非対応版） ---
typedef struct named_counter {
    char                  name[64];
    int64_t               value;
    struct named_counter *next;
} named_counter_t;

static named_counter_t *g_named_counters = NULL;

static named_counter_t* find_named_counter(const char *name) {
    for (named_counter_t *p = g_named_counters; p; p = p->next) {
        if (strncmp(p->name, name, sizeof(p->name)-1) == 0)
            return p;
    }
    return NULL;
}

void ds_metrics_increment(const char *name) {
    if (!name) return;
    named_counter_t *c = find_named_counter(name);
    if (!c) {
        named_counter_t *new_c = calloc(1, sizeof(*new_c));
        if (!new_c) return;  // メモリ確保失敗時は何もしない
        strncpy(new_c->name, name, sizeof(new_c->name)-1);
        new_c->name[sizeof(new_c->name)-1] = '\0';
        new_c->next = g_named_counters;
        g_named_counters = new_c;
        c = new_c;
    }
    c->value += 1;
}

int64_t ds_metrics_get(const char *name) {
    if (!name) return 0;
    named_counter_t *c = find_named_counter(name);
    return c ? c->value : 0;
}

/* 内部クリア関数：static linkage */
static void static_clear_named_metrics(void) {
    named_counter_t *p = g_named_counters;
    while (p) {
        named_counter_t *n = p->next;
        free(p);
        p = n;
    }
    g_named_counters = NULL;
}