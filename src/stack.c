#include "ds/stack.h"
#include "util/memory.h"        /* ds_malloc / ds_free ラッパー */
#include <string.h>             /* memcpy */

/* ───── 内部構造体（外部非公開） ───── */
struct ds_stack {
    void  **data;               /* 動的配列 */
    size_t  capacity;           /* 配列サイズ  */
    size_t  size;               /* 使用要素数 */
    ds_stats_t stats;           /* O(1) で返すために保持 */
};

#define DS_STACK_INIT_CAP 8

/* ───── ヘルパ ───── */
static ds_error_t grow_if_needed(const ds_allocator_t *alloc,
                                 ds_stack_t           *s);

/* ───── API 実装 ───── */

ds_error_t
ds_stack_create(const ds_allocator_t *alloc, ds_stack_t **out_s)
{
    if (!alloc || !out_s) return DS_ERR_NULL_POINTER;

    ds_stack_t *s = (ds_stack_t *)ds_malloc(alloc, 1, sizeof *s);
    if (!s) return DS_ERR_ALLOC;

    s->data = (void **)ds_malloc(alloc, DS_STACK_INIT_CAP, sizeof(void *));
    if (!s->data) { ds_free(alloc, s); return DS_ERR_ALLOC; }

    s->capacity = DS_STACK_INIT_CAP;
    s->size     = 0;
    memset(&s->stats, 0, sizeof s->stats);

    *out_s = s;
    return DS_SUCCESS;
}

ds_error_t
ds_stack_destroy(const ds_allocator_t *alloc, ds_stack_t *s)
{
    if (!alloc || !s) return DS_ERR_NULL_POINTER;

    ds_free(alloc, s->data);
    ds_free(alloc, s);
    return DS_SUCCESS;
}

ds_error_t
ds_stack_push(const ds_allocator_t *alloc, ds_stack_t *s, void *data)
{
    if (!alloc || !s)          return DS_ERR_NULL_POINTER;
    if (grow_if_needed(alloc, s) != DS_SUCCESS) return DS_ERR_ALLOC;

    s->data[s->size++] = data;
    s->stats.total_elements = s->size;
    s->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t
ds_stack_pop(const ds_allocator_t *alloc, ds_stack_t *s, void **out)
{
    (void)alloc;                       /* 現実装では解放なし */
    if (!s || !out)      return DS_ERR_NULL_POINTER;
    if (s->size == 0)    return DS_ERR_EMPTY;

    *out = s->data[--s->size];
    s->stats.total_elements = s->size;
    s->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t
ds_stack_peek(const ds_stack_t *s, void **out)
{
    if (!s || !out)      return DS_ERR_NULL_POINTER;
    if (s->size == 0)    return DS_ERR_EMPTY;

    *out = s->data[s->size - 1];
    return DS_SUCCESS;
}

bool
ds_stack_is_empty(const ds_stack_t *s) { return !s || s->size == 0; }

size_t
ds_stack_size(const ds_stack_t *s)     { return s ? s->size : 0; }

ds_error_t
ds_stack_reset(const ds_allocator_t *alloc, ds_stack_t *s)
{
    if (!alloc || !s) return DS_ERR_NULL_POINTER;
    s->size = 0;
    s->stats.total_elements = 0;
    s->stats.operations_count++;
    return DS_SUCCESS;
}

ds_error_t
ds_stack_get_stats(const ds_stack_t *s, ds_stats_t *stats)
{
    if (!s || !stats) return DS_ERR_NULL_POINTER;
    *stats = s->stats;
    return DS_SUCCESS;
}

/* ─────────── 内部関数 ─────────── */

static ds_error_t
grow_if_needed(const ds_allocator_t *alloc, ds_stack_t *s)
{
    if (s->size < s->capacity) return DS_SUCCESS;

    size_t new_cap = s->capacity * 2;
    void **newdata = (void **)ds_malloc(alloc, new_cap, sizeof(void *));
    if (!newdata) return DS_ERR_ALLOC;

    memcpy(newdata, s->data, s->size * sizeof(void *));
    ds_free(alloc, s->data);

    s->data     = newdata;
    s->capacity = new_cap;
    return DS_SUCCESS;
}
