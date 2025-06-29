#include "../../include/data_structures.h"
#include "../../include/ds/url_shortener.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
