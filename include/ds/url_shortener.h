#ifndef DS_URL_SHORTENER_H
#define DS_URL_SHORTENER_H
/*───────────────────────────────────────────────────────────*\
 *  URL Shortener Public API  ―  Opaque 型 + 抽象アロケータ DI
 *  © 2025  Example Corp.  |  API ver. 1.0
\*───────────────────────────────────────────────────────────*/

#include "data_structures.h"   /* ds_url_shortener_t / ds_error_t / ds_allocator_t */

#ifdef __cplusplus
extern "C" {
#endif

#define DS_URL_SHORTENER_API_VERSION "1.0"

/*======================================================================*
 *  コンストラクタ／デストラクタ
 *======================================================================*/

/**
 * @brief URL 短縮器インスタンス生成
 * @param[in]  alloc      抽象アロケータ（NULL=フォールバック）
 * @param[in]  capacity   内部 LRU キャッシュ容量（要: >0）
 * @param[out] out_us     生成されたインスタンス
 * @return                DS_SUCCESS / DS_ERR_xxx
 * @ownership caller frees via ::ds_url_shortener_destroy
 */
ds_error_t
ds_url_shortener_create(const ds_allocator_t *alloc,
                        size_t                capacity,
                        ds_url_shortener_t  **out_us);

/**
 * @brief URL 短縮器破棄
 * @param[in] alloc  同一アロケータ（NULL 不可）
 * @param[in] us     破棄対象
 * @return           エラーコード
 * @ownership callee frees
 */
ds_error_t
ds_url_shortener_destroy(const ds_allocator_t *alloc,
                         ds_url_shortener_t   *us);

/*======================================================================*
 *  Core Operations
 *======================================================================*/

/**
 * @brief 長い URL を短縮 ID へ変換
 * @param[in]  alloc       アロケータ
 * @param[in]  us          インスタンス
 * @param[in]  url         元 URL（NUL 終端）
 * @param[out] out_short   短縮 ID 出力バッファ
 * @param[in]  out_size    out_short のサイズ（≧ 7 推奨）
 * @return                 DS_SUCCESS / DS_ERR_xxx
 * @ownership caller owns out_short
 */
ds_error_t
ds_url_shortener_shorten(const ds_allocator_t *alloc,
                         ds_url_shortener_t   *us,
                         const char           *url,
                         char                 *out_short,
                         size_t                out_size);

/**
 * @brief 短縮 ID から元 URL を取得
 * @param[in]  alloc       アロケータ
 * @param[in]  us          インスタンス
 * @param[in]  short_id    短縮 ID（NUL 終端）
 * @param[out] out_url     復元 URL 出力バッファ
 * @param[in]  out_size    out_url のサイズ
 * @return                 DS_SUCCESS / DS_ERR_xxx
 * @ownership caller owns out_url
 */
ds_error_t
ds_url_shortener_expand(const ds_allocator_t *alloc,
                        ds_url_shortener_t   *us,
                        const char           *short_id,
                        char                 *out_url,
                        size_t                out_size);

#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif /* DS_URL_SHORTENER_H */