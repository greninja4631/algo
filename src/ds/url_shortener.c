<<<<<<< HEAD
<<<<<<< HEAD
#include "logger.h"
<<<<<<< HEAD
#include "../../include/data_structures.h"
#include "../../include/ds/url_shortener.h"
=======
#include <stdbool.h>
/**
 * @file url_shortener.c
 * @brief UuRL短縮サービスのサンプル（LRU+HashMap利用例、クラウド・WebAPI化容易）
 */
#include "data_structures.h"
#include "ds/lru_cache.h"
>>>>>>> feature
#include <stdio.h>
=======
=======
#include <stdio.h>
>>>>>>> feature
#include "ds/url_shortener.h"      // API宣言
#include "ds/lru_cache.h"
>>>>>>> feature
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

<<<<<<< HEAD
<<<<<<< HEAD
// ---- 構造体定義 ----

struct ds_url_shortener {
    char original_urls[100][256];
    char short_urls[100][16];
    size_t count;
};

// ---- ユーティリティ ----

static void generate_short_code(const char* url, char* out, size_t out_size) {
    unsigned int hash = 5381;
    for (const char* p = url; *p; ++p)
        hash = ((hash << 5) + hash) + (unsigned char)(*p);
    snprintf(out, out_size, "%08X", hash & 0xFFFFFFFF);
}

// ---- API本体 ----

ds_url_shortener_t* ds_url_shortener_create(void) {
    ds_url_shortener_t* us = (ds_url_shortener_t*)ds_malloc(sizeof(ds_url_shortener_t));
    if (!us) {
        ds_log(DS_LOG_ERROR, "[url_shortener] メモリ確保失敗 (create)\n");
        return NULL;
    }
    memset(us, 0, sizeof(ds_url_shortener_t));
    return us;
}

ds_error_t ds_url_shortener_destroy(ds_url_shortener_t* us) {
    if (!us) return DS_ERR_NULL_POINTER;
    ds_free(us);
    return DS_SUCCESS;
}

ds_error_t ds_url_shortener_shorten(ds_url_shortener_t* us, const char* url, char* out_short, size_t out_size) {
    if (!us || !url || !out_short || out_size == 0) {
        ds_log(DS_LOG_ERROR, "[url_shortener] 無効な引数 (shorten)\n");
        return DS_ERR_INVALID_ARG;
    }
    // すでに短縮済みかチェック
    for (size_t i = 0; i < us->count; ++i) {
        if (strcmp(us->original_urls[i], url) == 0) {
            strncpy(out_short, us->short_urls[i], out_size-1);
            out_short[out_size-1] = '\0';
            return DS_SUCCESS;
        }
    }
    if (us->count >= 100) {
        ds_log(DS_LOG_ERROR, "[url_shortener] 最大保存件数を超えました\n");
        return DS_ERR_OVERFLOW;
    }
    strncpy(us->original_urls[us->count], url, 255);
    us->original_urls[us->count][255] = '\0';
    generate_short_code(url, us->short_urls[us->count], sizeof(us->short_urls[us->count]));
    strncpy(out_short, us->short_urls[us->count], out_size-1);
    out_short[out_size-1] = '\0';
    us->count++;
    return DS_SUCCESS;
}

ds_error_t ds_url_shortener_expand(ds_url_shortener_t* us, const char* short_url, char* out_url, size_t out_size) {
    if (!us || !short_url || !out_url || out_size == 0) {
        ds_log(DS_LOG_ERROR, "[url_shortener] 無効な引数 (expand)\n");
        return DS_ERR_INVALID_ARG;
    }
    for (size_t i = 0; i < us->count; ++i) {
        if (strcmp(us->short_urls[i], short_url) == 0) {
            strncpy(out_url, us->original_urls[i], out_size-1);
            out_url[out_size-1] = '\0';
            return DS_SUCCESS;
        }
    }
    ds_log(DS_LOG_WARN, "[url_shortener] 該当する短縮URLが見つかりません: %s\n", short_url);
    return DS_ERR_NOT_FOUND;
}
=======
// 超シンプルなID生成（現場ではハッシュ化/乱数/UUID推奨）
=======
// ---- 内部ユーティリティ ----
>>>>>>> feature
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

<<<<<<< HEAD
/// 短縮ID→元URLを取得
const char* url_shortener_get(url_shortener_t* s, const char* id) {
    void* url = NULL;
    if (lru_cache_get(s->cache, id, &url) == HM_OK)
        return (const char*)url;
    return NULL;
}
>>>>>>> feature
=======
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
>>>>>>> feature
