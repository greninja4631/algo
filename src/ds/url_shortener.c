<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
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
=======
// src/ds/url_shortener.c
=======
>>>>>>> feature
=======
#include <stddef.h>
>>>>>>> feature
#include "ds/url_shortener.h"
#include "util/logger.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// 本体定義（この.cでのみ）
struct ds_url_shortener {
    int placeholder;
};

/*  DI allocator  */
static void* _alloc(size_t n,size_t sz){ return n?calloc(n,sz):NULL; }
static void  _free (void* p){ if(p) free(p); }
static const ds_allocator_t G_ALLOC_IMPL = { .alloc=_alloc, .free=_free };
#define G_ALLOC (&G_ALLOC_IMPL)

/*  tiny assert  */
#define TASSERT(c,m) do{ if(c) ds_log(DS_LOG_LEVEL_INFO,"[PASS] %s",(m)); \
                         else { ds_log(DS_LOG_LEVEL_ERROR,"[FAIL] %s",(m)); assert(0);} }while(0)
// API実装（引数・エラー処理・ownership完全統一）
// --------------------
ds_error_t
ds_url_shortener_create(const ds_allocator_t *alloc,
                        size_t                capacity,
                        ds_url_shortener_t  **out_us)
{
<<<<<<< HEAD
<<<<<<< HEAD
    /* 000123 のような 6 桁ゼロパディング */
    (void)snprintf(out, len, "%06d", id);
>>>>>>> feature
}
=======
    ds_url_shortener_t *us=NULL;
    TASSERT(ds_url_shortener_create(G_ALLOC, 16, &us)==DS_SUCCESS,"create");
>>>>>>> feature
=======
    if (!out_us)       return DS_ERR_NULL_POINTER;
    if (capacity == 0) return DS_ERR_INVALID_ARG;
>>>>>>> feature

    void *(*my_alloc)(size_t, size_t) = CHOOSE_ALLOC(alloc);

    ds_url_shortener_t *us =
        (ds_url_shortener_t *)my_alloc(1, sizeof(struct ds_url_shortener));
    if (!us) return DS_ERR_ALLOC;

<<<<<<< HEAD
<<<<<<< HEAD
    ds_url_shortener_t *us = alloc->alloc(1, sizeof *us);
    if (!us) return DS_ERR_ALLOC;
    memset(us, 0, sizeof *us);

    us->alloc = alloc;
    ds_error_t rc = ds_lru_cache_create(alloc, capacity, &us->cache);
    if (rc != DS_SUCCESS) { alloc->free(us); return rc; }
=======
    us->placeholder = 0;   /* 必要なら初期化 */
>>>>>>> feature

    *out_us = us;
    return DS_SUCCESS;
}

<<<<<<< HEAD
ds_error_t
ds_url_shortener_destroy(ds_url_shortener_t *us)
{
    if (!us) return DS_ERR_NULL_POINTER;
    ds_lru_cache_destroy(us->alloc, us->cache);
    us->alloc->free(us);
    return DS_SUCCESS;
}

<<<<<<< HEAD
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
=======
/* URL → 短縮 ID */
ds_error_t
ds_url_shortener_shorten(ds_url_shortener_t *us,
                         const char         *url,
                         char               *out_short,
                         size_t              out_sz)
{
>>>>>>> feature
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
<<<<<<< HEAD
>>>>>>> feature
=======
>>>>>>> feature
=======
    ds_url_shortener_destroy(G_ALLOC, us);
}
>>>>>>> feature
=======
/*---------------------------------------------------------------*
 *  破棄
 *---------------------------------------------------------------*/
ds_error_t
ds_url_shortener_destroy(const ds_allocator_t *alloc,
                         ds_url_shortener_t   *us)
{
    if (!us) return DS_ERR_NULL_POINTER;

    void (*my_free)(void *) = CHOOSE_FREE(alloc);
    my_free(us);
    return DS_SUCCESS;
}

/*---------------------------------------------------------------*
 *  短縮
 *---------------------------------------------------------------*/
ds_error_t
ds_url_shortener_shorten(const ds_allocator_t *alloc,   /* NOLINT(readability-non-const-parameter) */
                         ds_url_shortener_t   *us,
                         const char           *url,
                         char                 *out_short,
                         size_t                out_size)
{
    (void)alloc; /* 将来 DB / キャッシュ利用を予定 */

    if (!us || !url || !out_short || out_size < 2)
        return DS_ERR_INVALID_ARG;

    /* ★ダミー実装 – 実際はハッシュや連番に置換 */
    strncpy(out_short, "abc123", out_size - 1);
    out_short[out_size - 1] = '\0';

    return DS_SUCCESS;
}

/*---------------------------------------------------------------*
 *  展開
 *---------------------------------------------------------------*/
ds_error_t
ds_url_shortener_expand(const ds_allocator_t *alloc,   /* NOLINT(readability-non-const-parameter) */
                        ds_url_shortener_t   *us,
                        const char           *short_id,
                        char                 *out_url,
                        size_t                out_size)
{
    (void)alloc; /* 同上 */

    if (!us || !short_id || !out_url || out_size < 2)
        return DS_ERR_INVALID_ARG;

    /* ★ダミー実装 – 実際はマップを参照 */
    strncpy(out_url, "https://example.com/aaaaaaaa", out_size - 1);
    out_url[out_size - 1] = '\0';

    return DS_SUCCESS;
}
>>>>>>> feature
