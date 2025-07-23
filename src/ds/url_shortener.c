/**
 * @file    src/ds/url_shortener.c
 * @brief   URL 短縮サービス – 実装（Opaque + 抽象アロケータ DI）
 *
 * ガイドライン 2025-07 準拠：
 *  - 直 malloc/calloc/free を使用しない（ds_malloc/ds_free）
 *  - 公開 API 第 1 引数は const ds_allocator_t *alloc 固定
 *  - @pre/@post で契約プログラミング
 */

#include <stddef.h>           /* size_t */
#include <assert.h>
#include <string.h>

#include "ds/url_shortener.h"
#include "util/logger.h"
#include "util/memory.h"      /* ds_malloc / ds_free */

/*─────────────────────────────────────────────────────────────*
 *  内部型（Opaque 本体）
 *─────────────────────────────────────────────────────────────*/
struct ds_url_shortener {
    const ds_allocator_t *alloc;   /* @ownership caller provides */
    size_t                capacity;
};

/*─────────────────────────────────────────────────────────────*
 *  フォールバックアロケータ（alloc==NULL 時に使用）
 *─────────────────────────────────────────────────────────────*/
static void *
fallback_alloc(size_t n, size_t sz)
{
    /* ds_malloc(NULL,…) は util/memory.h 側で calloc フォールバック */
    return ds_malloc(/*alloc=*/NULL, n, sz);
}

static void
fallback_free(void *p)
{
    ds_free(/*alloc=*/NULL, p);
}

/*─────────────────────────────────────────────────────────────*
 *  マクロ：呼び出し側 DI / フォールバック選択
 *─────────────────────────────────────────────────────────────*/
#define CHOOSE_ALLOC(A) ((A) && (A)->alloc ? (A)->alloc : fallback_alloc)
#define CHOOSE_FREE(A)  ((A) && (A)->free  ? (A)->free  : fallback_free)

#define LOG_FAIL_AND_RETURN(code, msg) \
    do { ds_log(DS_LOG_LEVEL_ERROR, "%s", (msg)); return (code); } while (0)

/*─────────────────────────────────────────────────────────────*
 *  生成
 *─────────────────────────────────────────────────────────────*/
/**
 * @pre   out_us   != NULL
 * @pre   capacity > 0
 * @post  *out_us  != NULL
 * @ownership caller frees via ds_url_shortener_destroy
 */
ds_error_t
ds_url_shortener_create(const ds_allocator_t *alloc,
                        size_t                capacity,
                        ds_url_shortener_t  **out_us)
{
    assert(out_us);
    if (!out_us)       return DS_ERR_NULL_POINTER;
    if (capacity == 0) return DS_ERR_INVALID_ARG;

    ds_url_shortener_t *us =
        (ds_url_shortener_t *)CHOOSE_ALLOC(alloc)(1, sizeof *us);
    if (!us)           return DS_ERR_ALLOC;

    us->alloc    = alloc;
    us->capacity = capacity;
    *out_us      = us;
    return DS_SUCCESS;
}

/*─────────────────────────────────────────────────────────────*
 *  破棄
 *─────────────────────────────────────────────────────────────*/
ds_error_t
ds_url_shortener_destroy(const ds_allocator_t *alloc,
                         ds_url_shortener_t   *us)
{
    if (!us) return DS_ERR_NULL_POINTER;
    CHOOSE_FREE(alloc)(us);
    return DS_SUCCESS;
}

/*─────────────────────────────────────────────────────────────*
 *  URL → 短縮 ID 変換
 *─────────────────────────────────────────────────────────────*/
ds_error_t
ds_url_shortener_shorten(const ds_allocator_t *alloc, /* 今は未使用 */
                         ds_url_shortener_t   *us,
                         const char           *url,
                         char                 *out_short,
                         size_t                out_size)
{
    (void)alloc; /* 予約：将来 DB / キャッシュ利用 */

    if (!us || !url || !out_short || out_size < 2)
        LOG_FAIL_AND_RETURN(DS_ERR_INVALID_ARG,
                            "[url_shortener] shorten: invalid arg");

    /* ★ダミー実装 – 実装フェーズでハッシュ or 連番を生成 */
    strncpy(out_short, "abc123", out_size - 1);
    out_short[out_size - 1] = '\0';
    return DS_SUCCESS;
}

/*─────────────────────────────────────────────────────────────*
 *  短縮 ID → URL 変換
 *─────────────────────────────────────────────────────────────*/
ds_error_t
ds_url_shortener_expand(const ds_allocator_t *alloc, /* 今は未使用 */
                        ds_url_shortener_t   *us,
                        const char           *short_id,
                        char                 *out_url,
                        size_t                out_size)
{
    (void)alloc; /* 同上 */

    if (!us || !short_id || !out_url || out_size < 2)
        LOG_FAIL_AND_RETURN(DS_ERR_INVALID_ARG,
                            "[url_shortener] expand: invalid arg");

    /* ★ダミー実装 – 実装フェーズでマップ参照 */
    strncpy(out_url, "https://example.com/aaaaaaaa", out_size - 1);
    out_url[out_size - 1] = '\0';
    return DS_SUCCESS;
}
