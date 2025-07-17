#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include "util/metrics.h"

struct ds_metrics {
    size_t   total_elements;
    size_t   memory_allocated;
    size_t   operations_count;
    uint64_t creation_timestamp;
};

/* --- Opaque管理 --- */
ds_error_t ds_metrics_init(const ds_allocator_t* alloc, ds_metrics_t* metrics) {
    if (!metrics) return DS_ERR_NULL_POINTER;
    memset(metrics, 0, sizeof(ds_metrics_t));
    metrics->creation_timestamp = (uint64_t)time(NULL);
    return DS_SUCCESS;
}

ds_error_t ds_metrics_increment_ops(const ds_allocator_t* alloc, ds_metrics_t* metrics) {
    if (!metrics) return DS_ERR_NULL_POINTER;
    metrics->operations_count++;
    return DS_SUCCESS;
}

ds_error_t ds_metrics_increment_elements(const ds_allocator_t* alloc, ds_metrics_t* metrics) {
    if (!metrics) return DS_ERR_NULL_POINTER;
    metrics->total_elements++;
    return DS_SUCCESS;
}

ds_error_t ds_metrics_decrement_elements(const ds_allocator_t* alloc, ds_metrics_t* metrics) {
    if (!metrics) return DS_ERR_NULL_POINTER;
    if (metrics->total_elements > 0) metrics->total_elements--;
    return DS_SUCCESS;
}

ds_error_t ds_metrics_add_memory(const ds_allocator_t* alloc, ds_metrics_t* metrics, size_t bytes) {
    if (!metrics) return DS_ERR_NULL_POINTER;
    metrics->memory_allocated += bytes;
    return DS_SUCCESS;
}

ds_error_t ds_metrics_sub_memory(const ds_allocator_t* alloc, ds_metrics_t* metrics, size_t bytes) {
    if (!metrics) return DS_ERR_NULL_POINTER;
    if (metrics->memory_allocated >= bytes) metrics->memory_allocated -= bytes;
    else metrics->memory_allocated = 0;
    return DS_SUCCESS;
}

/* --- メトリクスの可視化 --- */
void ds_metrics_print(const ds_allocator_t* alloc, const ds_metrics_t* metrics) {
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

/* --- 全メトリクスリセット（テスト用） --- */
static void static_clear_named_metrics(const ds_allocator_t* alloc);
void ds_metrics_reset_all(const ds_allocator_t* alloc) {
    static_clear_named_metrics(alloc);
}

/* --- 名前付きカウンタ --- */
typedef struct named_counter {
    char name[64];
    int64_t value;
    struct named_counter* next;
} named_counter_t;

static named_counter_t* g_named_counters = NULL;

static named_counter_t* find_named_counter(const char* name) {
    for (named_counter_t* p = g_named_counters; p; p = p->next) {
        if (strncmp(p->name, name, sizeof(p->name) - 1) == 0)
            return p;
    }
    return NULL;
}

void ds_metrics_increment(const ds_allocator_t* alloc, const char* name) {
    if (!name || !alloc) return;
    named_counter_t* c = find_named_counter(name);
    if (!c) {
        named_counter_t* new_c = alloc->alloc(1, sizeof(*new_c));
        if (!new_c) return;
        strncpy(new_c->name, name, sizeof(new_c->name) - 1);
        new_c->name[sizeof(new_c->name) - 1] = '\0';
        new_c->value = 0;
        new_c->next = g_named_counters;
        g_named_counters = new_c;
        c = new_c;
    }
    c->value += 1;
}

int64_t ds_metrics_get(const ds_allocator_t* alloc, const char* name) {
    if (!name) return 0;
    named_counter_t* c = find_named_counter(name);
    return c ? c->value : 0;
}

static void static_clear_named_metrics(const ds_allocator_t* alloc) {
    named_counter_t* p = g_named_counters;
    while (p) {
        named_counter_t* n = p->next;
        if (alloc) alloc->free(p);
        p = n;
    }
    g_named_counters = NULL;
}
