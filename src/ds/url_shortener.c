#include <stddef.h>
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
    if (!out_us)       return DS_ERR_NULL_POINTER;
    if (capacity == 0) return DS_ERR_INVALID_ARG;

    void *(*my_alloc)(size_t, size_t) = CHOOSE_ALLOC(alloc);

    ds_url_shortener_t *us =
        (ds_url_shortener_t *)my_alloc(1, sizeof(struct ds_url_shortener));
    if (!us) return DS_ERR_ALLOC;

    us->placeholder = 0;   /* 必要なら初期化 */

    *out_us = us;
    return DS_SUCCESS;
}

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