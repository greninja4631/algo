/**
 * @file url_shortener.c
 * @brief URL短縮サービスのサンプル（LRU+HashMap利用例、クラウド・WebAPI化容易）
 */
#include "lru_cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 超シンプルなID生成（現場ではハッシュ化/乱数/UUID推奨）
static int next_id = 1;
static void encode_id(int id, char* out, size_t outlen) {
    snprintf(out, outlen, "%06d", id); // 6桁ゼロパディング
}

/// サービス構造体
typedef struct {
    lru_cache_t* cache;
} url_shortener_t;

/// API設計例
url_shortener_t* url_shortener_create(size_t cap) {
    url_shortener_t* s = calloc(1, sizeof(url_shortener_t));
    s->cache = lru_cache_create(cap);
    return s;
}
void url_shortener_destroy(url_shortener_t* s) {
    if (!s) return;
    lru_cache_destroy(s->cache);
    free(s);
}

/// 新規URL追加（短縮ID返す）
char* url_shortener_add(url_shortener_t* s, const char* url) {
    char* id = malloc(8);
    encode_id(next_id++, id, 8);
    lru_cache_put(s->cache, id, strdup(url));
    return id;
}

/// 短縮ID→元URLを取得
const char* url_shortener_get(url_shortener_t* s, const char* id) {
    void* url = NULL;
    if (lru_cache_get(s->cache, id, &url) == HM_OK)
        return (const char*)url;
    return NULL;
}