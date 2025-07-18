/*======================================================================*
 *  src/ds/lru_cache.c
 *  Allocator-aware 非スレッドセーフ LRUキャッシュ（Opaque型＋DI徹底）
 *  2025-07 API設計・実装ガイドライン完全準拠
 *======================================================================*/
#include "ds/lru_cache.h"
#include "ds/hashmap.h"
#include "util/memory.h"
#include <string.h>
#include <stddef.h>

/*────────────────────── 内部型 ──────────────────────*/
typedef struct ds_lru_node {
    char               *key;
    void               *value;
    struct ds_lru_node *prev;
    struct ds_lru_node *next;
} ds_lru_node_t;

struct ds_lru_cache {
    size_t           capacity;
    size_t           size;
    ds_lru_node_t   *head;
    ds_lru_node_t   *tail;
    ds_hashmap_t    *map;
};

/*────────────────────── 内部 forward ──────────────────────*/
static ds_lru_node_t *_move_to_head(ds_lru_cache_t *c, ds_lru_node_t *n);
static void           _evict_tail(const ds_allocator_t *alloc, ds_lru_cache_t *c);
static char          *_strdup_alloc(const ds_allocator_t *a, const char *s);

/*======================================================================*
 *  PUBLIC API
 *======================================================================*/

ds_error_t ds_lru_cache_create(const ds_allocator_t *alloc, size_t capacity, ds_lru_cache_t **out_cache)
{
    if (!alloc || !out_cache || capacity == 0) return DS_ERR_INVALID_ARG;
    ds_lru_cache_t *c = ds_malloc(alloc, 1, sizeof *c);
    if (!c) return DS_ERR_ALLOC;
    c->capacity = capacity;
    c->size     = 0;
    c->head = c->tail = NULL;
    ds_error_t rc = ds_hashmap_create(alloc, capacity, NULL, NULL, &c->map);
    if (rc != DS_SUCCESS) { ds_free(alloc, c); return rc; }
    *out_cache = c;
    return DS_SUCCESS;
}

ds_error_t ds_lru_cache_destroy(const ds_allocator_t *alloc, ds_lru_cache_t *c)
{
    if (!alloc || !c) return DS_ERR_NULL_POINTER;
    // ノード全free
    ds_lru_node_t *n = c->head;
    while (n) {
        ds_lru_node_t *next = n->next;
        ds_free(alloc, n->key);
        ds_free(alloc, n);
        n = next;
    }
    // マップもfree
    ds_hashmap_destroy(alloc, c->map);
    ds_free(alloc, c);
    return DS_SUCCESS;
}

ds_error_t ds_lru_cache_put(const ds_allocator_t *alloc, ds_lru_cache_t *c, const char *key, void *value)
{
    if (!alloc || !c || !key) return DS_ERR_NULL_POINTER;
    void *hv = NULL;
    if (ds_hashmap_get(c->map, key, &hv) == DS_SUCCESS) {
        ds_lru_node_t *n = hv;
        n->value = value;
        _move_to_head(c, n);
        return DS_SUCCESS;
    }
    ds_lru_node_t *n = ds_malloc(alloc, 1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;
    n->key = _strdup_alloc(alloc, key);
    if (!n->key) { ds_free(alloc, n); return DS_ERR_ALLOC; }
    n->value = value;
    n->prev = NULL;
    n->next = c->head;
    if (c->head) c->head->prev = n;
    c->head = n;
    if (!c->tail) c->tail = n;
    ds_hashmap_put(alloc, c->map, n->key, n);
    c->size++;
    if (c->size > c->capacity) _evict_tail(alloc, c);
    return DS_SUCCESS;
}

ds_error_t ds_lru_cache_get(const ds_allocator_t *alloc, ds_lru_cache_t *c, const char *key, void **out_value)
{
    (void)alloc;
    if (!c || !key || !out_value) return DS_ERR_NULL_POINTER;
    void *hv = NULL;
    if (ds_hashmap_get(c->map, key, &hv) != DS_SUCCESS) return DS_ERR_NOT_FOUND;
    ds_lru_node_t *n = hv;
    _move_to_head(c, n);
    *out_value = n->value;
    return DS_SUCCESS;
}

ds_error_t ds_lru_cache_size(const ds_allocator_t *alloc, const ds_lru_cache_t *c, size_t *out_size)
{
    (void)alloc;
    if (!c || !out_size) return DS_ERR_NULL_POINTER;
    *out_size = c->size;
    return DS_SUCCESS;
}

/*======================================================================*
 *  INTERNAL HELPERS
 *======================================================================*/

static ds_lru_node_t *_move_to_head(ds_lru_cache_t *c, ds_lru_node_t *n)
{
    if (c->head == n) return n;
    if (n->prev) n->prev->next = n->next;
    if (n->next) n->next->prev = n->prev;
    if (c->tail == n) c->tail = n->prev;
    n->prev = NULL;
    n->next = c->head;
    if (c->head) c->head->prev = n;
    c->head = n;
    if (!c->tail) c->tail = n;
    return n;
}

static void _evict_tail(const ds_allocator_t *alloc, ds_lru_cache_t *c)
{
    ds_lru_node_t *t = c->tail;
    if (!t) return;
    if (t->prev) t->prev->next = NULL;
    c->tail = t->prev;
    ds_hashmap_remove(alloc, c->map, t->key);
    ds_free(alloc, t->key);
    ds_free(alloc, t);
    c->size--;
}

static char *_strdup_alloc(const ds_allocator_t *alloc, const char *s)
{
    size_t len = strlen(s) + 1;
    char *dup = ds_malloc(alloc, len, sizeof(char));
    if (dup) memcpy(dup, s, len);
    return dup;
}