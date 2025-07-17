// src/ds/url_shortener.c
#include "ds/url_shortener.h"
#include "ds/lru_cache.h"
#include "util/memory.h"

#include <string.h>
#include <stdio.h>

/* ───── 内部ユーティリティ ───── */
static int  g_next_id = 1;
static void encode_id(int id, char *out, size_t len)
{
    /* 000123 のような 6 桁ゼロパディング */
    (void)snprintf(out, len, "%06d", id);
}

/* ───── Opaque 本体（.c 限定） ───── */
struct ds_url_shortener {
    const ds_allocator_t *alloc;   /* DI アロケータ */
    ds_lru_cache_t       *cache;   /* ID ↔ URL */
};

/* ───── API 実装 ───── */
ds_error_t
ds_url_shortener_create(const ds_allocator_t *alloc,
                        size_t                capacity,
                        ds_url_shortener_t  **out_us)
{
    if (!alloc || !out_us) return DS_ERR_NULL_POINTER;

    ds_url_shortener_t *us = alloc->alloc(1, sizeof *us);
    if (!us) return DS_ERR_ALLOC;
    memset(us, 0, sizeof *us);

    us->alloc = alloc;
    ds_error_t rc = ds_lru_cache_create(alloc, capacity, &us->cache);
    if (rc != DS_SUCCESS) { alloc->free(us); return rc; }

    *out_us = us;
    return DS_SUCCESS;
}

ds_error_t
ds_url_shortener_destroy(ds_url_shortener_t *us)
{
    if (!us) return DS_ERR_NULL_POINTER;
    ds_lru_cache_destroy(us->alloc, us->cache);
    us->alloc->free(us);
    return DS_SUCCESS;
}

/* URL → 短縮 ID */
ds_error_t
ds_url_shortener_shorten(ds_url_shortener_t *us,
                         const char         *url,
                         char               *out_short,
                         size_t              out_sz)
{
    if (!us || !url || !out_short) return DS_ERR_NULL_POINTER;

    char id[16];
    encode_id(g_next_id++, id, sizeof id);

    /* URL を DI アロケータで複製（strdup は直書き禁止） */
    size_t len = strlen(url) + 1;
    char *url_copy = us->alloc->alloc(len, 1);
    if (!url_copy) return DS_ERR_ALLOC;
    memcpy(url_copy, url, len);

    ds_error_t rc = ds_lru_cache_put(us->alloc, us->cache, id, url_copy);
    if (rc != DS_SUCCESS) { us->alloc->free(url_copy); return rc; }

    strncpy(out_short, id, out_sz);
    out_short[out_sz - 1] = '\0';
    return DS_SUCCESS;
}

/* 短縮 ID → 元 URL */
ds_error_t
ds_url_shortener_expand(ds_url_shortener_t *us,
                        const char         *short_id,
                        char               *out_url,
                        size_t              out_sz)
{
    if (!us || !short_id || !out_url) return DS_ERR_NULL_POINTER;

    void *url_ptr = NULL;
    ds_error_t rc = ds_lru_cache_get(us->alloc, us->cache, short_id, &url_ptr);
    if (rc != DS_SUCCESS) return rc;

    strncpy(out_url, (const char *)url_ptr, out_sz);
    out_url[out_sz - 1] = '\0';
    return DS_SUCCESS;
}
