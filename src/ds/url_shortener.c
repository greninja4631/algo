#include <stdio.h>
#include "ds/url_shortener.h"      // API宣言
#include "ds/lru_cache.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ---- 内部ユーティリティ ----
static int next_id = 1;
static void encode_id(int id, char* out, size_t outlen) {
    snprintf(out, outlen, "%06d", id); // 6桁ゼロパディング
}

// ---- 内部構造体定義（.cのみ） ----
struct ds_url_shortener {
    ds_lru_cache_t* cache;
};

// ---- API実装 ----

ds_error_t ds_url_shortener_create(size_t capacity, ds_url_shortener_t** out_us) {
    if (!out_us) return DS_ERR_NULL_POINTER;
    ds_url_shortener_t* s = (ds_url_shortener_t*)calloc(1, sizeof(ds_url_shortener_t));
    if (!s) return DS_ERR_ALLOC;
    ds_error_t err = ds_lru_cache_create(capacity, &s->cache);
    if (err != DS_SUCCESS) { free(s); return err; }
    *out_us = s;
    return DS_SUCCESS;
}

ds_error_t ds_url_shortener_destroy(ds_url_shortener_t* us) {
    if (!us) return DS_ERR_NULL_POINTER;
    ds_lru_cache_destroy(us->cache);
    free(us);
    return DS_SUCCESS;
}

ds_error_t ds_url_shortener_shorten(ds_url_shortener_t* us, const char* url, char* out_short, size_t out_size) {
    if (!us || !url || !out_short) return DS_ERR_NULL_POINTER;
    char id[16];
    encode_id(next_id++, id, sizeof(id));
    ds_error_t err = ds_lru_cache_put(us->cache, id, strdup(url));
    if (err != DS_SUCCESS) return err;
    strncpy(out_short, id, out_size);
    out_short[out_size - 1] = '\0';
    return DS_SUCCESS;
}

ds_error_t ds_url_shortener_expand(ds_url_shortener_t* us, const char* short_url, char* out_url, size_t out_size) {
    if (!us || !short_url || !out_url) return DS_ERR_NULL_POINTER;
    void* url_ptr = NULL;
    ds_error_t err = ds_lru_cache_get(us->cache, short_url, &url_ptr);
    if (err != DS_SUCCESS) return err;
    strncpy(out_url, (const char*)url_ptr, out_size);
    out_url[out_size - 1] = '\0';
    return DS_SUCCESS;
}