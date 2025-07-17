/*======================================================================*
 *  src/ds/lru_cache.c
 *  抽象アロケータ DI 対応・スレッド非安全版 LRU キャッシュ実装
 *======================================================================*/
#include "ds/lru_cache.h"
#include "ds/hashmap.h"      /* key→node の O(1) 参照用 */
#include "util/memory.h"     /* ds_malloc / ds_free      */
#include <string.h>          /* strlen / memcpy          >
#include <stddef.h>          /* NULL                     */

/*────────────────────── 内部型 ──────────────────────*/
typedef struct ds_lru_node {
    char                *key;      /* dup したキー文字列 (alloc 経由) */
    void                *value;
    struct ds_lru_node  *prev;     /* Doubly-linked list for O(1) ops */
    struct ds_lru_node  *next;
} ds_lru_node_t;

struct ds_lru_cache {              /* ← Opaque 実体：外部公開しない */
    const ds_allocator_t *alloc;   /* DI-アロケータ (必ず非 NULL)    */
    size_t                capacity;
    size_t                size;
    ds_lru_node_t        *head;    /* 先頭＝Most-Recently-Used       */
    ds_lru_node_t        *tail;    /* 末尾＝Least-Recently-Used      */
    ds_hashmap_t         *map;     /* key → ds_lru_node_t*           */
};

/*────────────────────── 内部 forward ──────────────────────*/
static ds_lru_node_t *_move_to_head(ds_lru_cache_t *c, ds_lru_node_t *n);
static void           _evict_tail  (ds_lru_cache_t *c);
static char          *_strdup_alloc(const ds_allocator_t *a, const char *s);

/*======================================================================*
 *  Public  API
 *======================================================================*/
ds_error_t
ds_lru_cache_create(const ds_allocator_t *alloc,
                    size_t                capacity,
                    ds_lru_cache_t      **out_cache)
{
    if (!alloc || !out_cache || capacity == 0) return DS_ERR_INVALID_ARG;

    ds_lru_cache_t *c = ds_malloc(alloc, 1, sizeof *c);
    if (!c) return DS_ERR_ALLOC;

    c->alloc    = alloc;
    c->capacity = capacity;
    c->size     = 0;
    c->head = c->tail = NULL;

    /* key/val の解放は LRU 側で行うので、ハッシュマップには NULL を渡す */
    ds_error_t rc = ds_hashmap_create(
        alloc, capacity, NULL, NULL, &c->map);
    if (rc != DS_SUCCESS) { ds_free(alloc, c); return rc; }

    *out_cache = c;
    return DS_SUCCESS;
}

ds_error_t
ds_lru_cache_destroy(const ds_allocator_t *alloc, ds_lru_cache_t *c)
{
    if (!alloc || !c) return DS_ERR_NULL_POINTER;

    for (ds_lru_node_t *n = c->head; n; ) {
        ds_lru_node_t *next = n->next;
        ds_free(alloc, n->key);
        ds_free(alloc, n);
        n = next;
    }
    ds_hashmap_destroy(alloc, c->map);
    ds_free(alloc, c);
    return DS_SUCCESS;
}

ds_error_t
ds_lru_cache_put(const ds_allocator_t *alloc,
                 ds_lru_cache_t       *c,
                 const char           *key,
                 void                 *value)
{
    if (!alloc || !c || !key) return DS_ERR_NULL_POINTER;

    /* 既存ノードなら値更新＋MRU 移動 */
    void *hv = NULL;
    if (ds_hashmap_get(c->map, key, &hv) == DS_SUCCESS) {
        ds_lru_node_t *n = hv;
        n->value = value;
        _move_to_head(c, n);
        return DS_SUCCESS;
    }

    /* 新規ノードを確保 */
    ds_lru_node_t *n = ds_malloc(alloc, 1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;

    n->key = _strdup_alloc(alloc, key);
    if (!n->key) { ds_free(alloc, n); return DS_ERR_ALLOC; }
    n->value = value;

    /* MRU 位置へ挿入 */
    n->prev = NULL;
    n->next = c->head;
    if (c->head) c->head->prev = n;
    c->head = n;
    if (!c->tail) c->tail = n;

    ds_hashmap_put(alloc, c->map, n->key, n); /* 失敗しない前提 */

    if (++c->size > c->capacity) _evict_tail(c);
    return DS_SUCCESS;
}

ds_error_t
ds_lru_cache_get(ds_lru_cache_t *c, const char *key, void **out_value)
{
    if (!c || !key || !out_value) return DS_ERR_NULL_POINTER;

    void *hv = NULL;
    if (ds_hashmap_get(c->map, key, &hv) != DS_SUCCESS) return DS_ERR_NOT_FOUND;

    ds_lru_node_t *n = hv;
    _move_to_head(c, n);
    *out_value = n->value;
    return DS_SUCCESS;
}

ds_error_t
ds_lru_cache_size(const ds_lru_cache_t *c, size_t *out_size)
{
    if (!c || !out_size) return DS_ERR_NULL_POINTER;
    *out_size = c->size;
    return DS_SUCCESS;
}

/*======================================================================*
 *  Internal helpers
 *======================================================================*/
static ds_lru_node_t *
_move_to_head(ds_lru_cache_t *c, ds_lru_node_t *n)
{
    if (c->head == n) return n;          /* 既に MRU */

    /* detach */
    if (n->prev) n->prev->next = n->next;
    if (n->next) n->next->prev = n->prev;
    if (c->tail == n) c->tail  = n->prev;

    /* insert at head */
    n->prev = NULL;
    n->next = c->head;
    if (c->head) c->head->prev = n;
    c->head = n;
    if (!c->tail) c->tail = n;
    return n;
}

/* capacity 超過時の LRU 削除 */
static void
_evict_tail(ds_lru_cache_t *c)
{
    ds_lru_node_t *t = c->tail;
    if (!t) return;

    if (t->prev) t->prev->next = NULL;
    c->tail = t->prev;
    ds_hashmap_remove(c->alloc, c->map, t->key);
    ds_free(c->alloc, t->key);
    ds_free(c->alloc, t);
    --c->size;
}

/* alloc 版 strdup （NUL 終端込み）*/
static char *
_strdup_alloc(const ds_allocator_t *alloc, const char *s)
{
    size_t len = strlen(s) + 1;
    char *dup  = ds_malloc(alloc, len, sizeof(char));
    if (dup) memcpy(dup, s, len);
    return dup;
}
