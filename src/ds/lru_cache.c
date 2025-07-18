<<<<<<< HEAD
#include "logger.h"
<<<<<<< HEAD
#include "../../include/data_structures.h"
#include "../../include/ds/lru_cache.h"
#include <stdlib.h>
#include <string.h>

// ノード構造体
typedef struct lru_node {
    int key;
    void* value;
    struct lru_node* prev;
    struct lru_node* next;
    struct lru_node* hnext;
} lru_node_t;

// 本体構造体
struct ds_lru_cache {
    size_t capacity;
    size_t size;
    lru_node_t* head;
    lru_node_t* tail;
    lru_node_t** map;
    size_t map_size;
};

// 内部ユーティリティ
static size_t hash(int key, size_t map_size) {
    return ((size_t)key) % map_size;
}

static void remove_node(ds_lru_cache_t* cache, lru_node_t* node) {
    if (node->prev) node->prev->next = node->next;
    else cache->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else cache->tail = node->prev;
    node->prev = node->next = NULL;
}

static void insert_front(ds_lru_cache_t* cache, lru_node_t* node) {
    node->prev = NULL;
=======
#include <stdlib.h>
#include <string.h>
=======
/*======================================================================*
 *  src/ds/lru_cache.c
 *  Allocator-aware 非スレッドセーフ LRUキャッシュ（Opaque型＋DI徹底）
 *  2025-07 API設計・実装ガイドライン完全準拠
 *======================================================================*/
>>>>>>> feature
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
<<<<<<< HEAD

<<<<<<< HEAD
<<<<<<< HEAD
    node = calloc(1, sizeof(ds_lru_node_t));
    node->key = strdup(key);
    node->value = value;
>>>>>>> feature
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
<<<<<<< HEAD
}

// --- 生成 ---
ds_lru_cache_t* ds_lru_cache_create(size_t capacity) {
    if (capacity == 0) {
        ds_log(DS_LOG_ERROR, "[lru_cache] キャパシティは1以上必要\n");
        return NULL;
    }
    ds_lru_cache_t* cache = (ds_lru_cache_t*)ds_malloc(sizeof(ds_lru_cache_t));
    if (!cache) {
        ds_log(DS_LOG_ERROR, "[lru_cache] 本体メモリ確保失敗\n");
        return NULL;
    }
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;
    cache->map_size = capacity * 4 + 7;
    cache->map = (lru_node_t**)ds_malloc(sizeof(lru_node_t*) * cache->map_size);
    if (!cache->map) {
        ds_log(DS_LOG_ERROR, "[lru_cache] map確保失敗\n");
        ds_free(cache);
        return NULL;
    }
    memset(cache->map, 0, sizeof(lru_node_t*) * cache->map_size);
    return cache;
}

// --- 破棄 ---
ds_error_t ds_lru_cache_destroy(ds_lru_cache_t* cache) {
    if (!cache) return DS_ERR_NULL_POINTER;
    lru_node_t* node = cache->head;
    while (node) {
        lru_node_t* next = node->next;
        if (node->value) ds_free(node->value); // 値もfreeする
        ds_free(node);
        node = next;
    }
    ds_free(cache->map);
    ds_free(cache);
    return DS_SUCCESS;
}

// --- 取得 ---
ds_error_t ds_lru_cache_get(ds_lru_cache_t* cache, int key, void** value) {
    if (!cache || !value) return DS_ERR_NULL_POINTER;
    size_t idx = hash(key, cache->map_size);
    lru_node_t* node = cache->map[idx];
    while (node) {
        if (node->key == key) {
            remove_node(cache, node);
            insert_front(cache, node);
            *value = node->value;
            return DS_SUCCESS;
        }
        node = node->hnext;
    }
    *value = NULL;
    ds_log(DS_LOG_INFO, "[lru_cache] キー%d見つからず\n", key);
    return DS_ERR_NOT_FOUND;
}

// --- 挿入/更新 ---
ds_error_t ds_lru_cache_put(ds_lru_cache_t* cache, int key, void* value) {
    if (!cache) return DS_ERR_NULL_POINTER;
    size_t idx = hash(key, cache->map_size);
    lru_node_t* node = cache->map[idx];

    // 既存keyなら値を更新して最前に移動
    while (node) {
        if (node->key == key) {
            if (node->value) ds_free(node->value);  // 以前の値をfree
            node->value = value;
            remove_node(cache, node);
            insert_front(cache, node);
            return DS_SUCCESS;
        }
        node = node->hnext;
    }

    // 容量超過時は末尾削除
    if (cache->size >= cache->capacity) {
        lru_node_t* del = cache->tail;
        if (del) {
            size_t del_idx = hash(del->key, cache->map_size);
            lru_node_t* p = cache->map[del_idx];
            lru_node_t* pprev = NULL;
            while (p) {
                if (p == del) {
                    if (pprev) pprev->hnext = p->hnext;
                    else cache->map[del_idx] = p->hnext;
                    break;
                }
                pprev = p;
                p = p->hnext;
            }
            remove_node(cache, del);
            if (del->value) ds_free(del->value);
            ds_free(del);
            cache->size--;
        }
    }

    // 新規ノード
    lru_node_t* newnode = (lru_node_t*)ds_malloc(sizeof(lru_node_t));
    if (!newnode) {
        ds_log(DS_LOG_ERROR, "[lru_cache] ノード確保失敗\n");
        return DS_ERR_OUT_OF_MEMORY;
    }
    newnode->key = key;
    newnode->value = value;
    newnode->prev = newnode->next = NULL;
    newnode->hnext = cache->map[idx];
    cache->map[idx] = newnode;
    insert_front(cache, newnode);
    cache->size++;
    return DS_SUCCESS;
=======
    ds_hashmap_put(cache->map, node->key, node);
    cache->size++;
=======
    // 新ノードを確保
    ds_lru_node_t *node = calloc(1, sizeof(*node));
    if (!node) return DS_ERR_ALLOC;
>>>>>>> feature
=======
    /* 新規ノードを確保 */
    ds_lru_node_t *n = ds_malloc(alloc, 1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;
>>>>>>> feature

=======
    ds_lru_node_t *n = ds_malloc(alloc, 1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;
>>>>>>> feature
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

<<<<<<< HEAD
<<<<<<< HEAD
// ──────────────────────────────────────
// 内部関数：容量超過時の末尾削除
// ──────────────────────────────────────
static void remove_tail(ds_lru_cache_t *cache) {
    ds_lru_node_t *tail = cache->tail;
    if (!tail) return;
    if (tail->prev) tail->prev->next = NULL;
    cache->tail = tail->prev;
    ds_hashmap_remove(cache->map, tail->key);
    free(tail->key);
    free(tail);
    cache->size--;
>>>>>>> feature
}
=======
/* capacity 超過時の LRU 削除 */
static void
_evict_tail(ds_lru_cache_t *c)
=======
static void _evict_tail(const ds_allocator_t *alloc, ds_lru_cache_t *c)
>>>>>>> feature
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
<<<<<<< HEAD
}
>>>>>>> feature
=======
}
>>>>>>> feature
