/*======================================================================*
 *  src/ds/hashmap.c  ―  FNV-1a / シングルチェイン方式の DI 対応ハッシュマップ
 *======================================================================*/
#include "ds/hashmap.h"
#include <string.h>     /* strcmp / strdup */
#include <stdint.h>

/* ───── 内部ノード & 本体 ───── */
typedef struct node {
    char        *key;
    void        *val;
    struct node *next;
} node_t;

struct ds_hashmap {
    size_t                 cap;
    size_t                 size;
    node_t              **bucket;      /* cap 要素の配列                            */
    ds_hashmap_free_func_t key_free;
    ds_hashmap_free_func_t val_free;
};

/* ───── FNV-1a 64-bit ───── */
static uint64_t hash_str(const char *s)
{
    uint64_t h = 14695981039346656037ULL;
    while (*s)
        h = (h ^ (unsigned char)*s++) * 1099511628211ULL;
    return h;
}

/* ───── create ───── */
ds_error_t
ds_hashmap_create(const ds_allocator_t  *a,
                  size_t                 cap,
                  ds_hashmap_free_func_t keyf,
                  ds_hashmap_free_func_t valf,
                  ds_hashmap_t         **out)
{
    if (!a || !out) return DS_ERR_NULL_POINTER;
    cap = cap ? cap : 16;

    ds_hashmap_t *m = a->alloc(1, sizeof *m);
    if (!m) return DS_ERR_ALLOC;

    m->bucket = a->alloc(cap, sizeof(node_t*));
    if (!m->bucket) { a->free(m); return DS_ERR_ALLOC; }

    m->cap = cap;
    m->size = 0;
    m->key_free = keyf;
    m->val_free = valf;
    *out = m;
    return DS_SUCCESS;
}

/* ───── destroy ───── */
ds_error_t
ds_hashmap_destroy(const ds_allocator_t *a, ds_hashmap_t *m)
{
    if (!a || !m) return DS_ERR_NULL_POINTER;

    for (size_t i = 0; i < m->cap; ++i) {
        node_t *n = m->bucket[i];
        while (n) {
            node_t *nx = n->next;
            if (m->key_free) m->key_free(n->key);
            if (m->val_free) m->val_free(n->val);
            a->free(n);
            n = nx;
        }
    }
    a->free(m->bucket);
    a->free(m);
    return DS_SUCCESS;
}

/* ───── put ───── */
ds_error_t
ds_hashmap_put(const ds_allocator_t *a,
               ds_hashmap_t         *m,
               const char           *key,
               void                 *val)
{
    if (!a || !m || !key) return DS_ERR_NULL_POINTER;

    size_t idx = hash_str(key) % m->cap;
    for (node_t *n = m->bucket[idx]; n; n = n->next)
        if (strcmp(n->key, key) == 0) return DS_ERR_INVALID_ARG; /* duplicate */

    node_t *n = a->alloc(1, sizeof *n);
    if (!n) return DS_ERR_ALLOC;

    char *dup = strdup(key);
    if (!dup) { a->free(n); return DS_ERR_ALLOC; }

    n->key = dup;
    n->val = val;
    n->next = m->bucket[idx];
    m->bucket[idx] = n;
    ++m->size;
    return DS_SUCCESS;
}

/* ───── get ───── */
ds_error_t
ds_hashmap_get(ds_hashmap_t *m, const char *key, void **out_val)
{
    if (!m || !key || !out_val) return DS_ERR_NULL_POINTER;
    size_t idx = hash_str(key) % m->cap;
    for (node_t *n = m->bucket[idx]; n; n = n->next)
        if (strcmp(n->key, key) == 0) { *out_val = n->val; return DS_SUCCESS; }
    return DS_ERR_NOT_FOUND;
}

/* ───── remove ───── */
ds_error_t
ds_hashmap_remove(const ds_allocator_t *a,
                  ds_hashmap_t         *m,
                  const char           *key)
{
    if (!a || !m || !key) return DS_ERR_NULL_POINTER;
    size_t idx = hash_str(key) % m->cap;
    node_t *cur = m->bucket[idx], *prev = NULL;

    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            if (prev) prev->next = cur->next;
            else      m->bucket[idx] = cur->next;

            if (m->key_free) m->key_free(cur->key);
            if (m->val_free) m->val_free(cur->val);
            a->free(cur);
            --m->size;
            return DS_SUCCESS;
        }
        prev = cur;
        cur  = cur->next;
    }
    return DS_ERR_NOT_FOUND;
}

/* ───── size ───── */
size_t ds_hashmap_size(const ds_hashmap_t *m)
{
    return m ? m->size : 0;
}
