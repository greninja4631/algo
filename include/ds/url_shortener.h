#ifndef DS_URL_SHORTENER_H
#define DS_URL_SHORTENER_H

#include "data_structures.h"  // ds_url_shortener_t, ds_error_t など

#ifdef __cplusplus
extern "C" {
#endif

#define DS_URL_SHORTENER_API_VERSION "1.0"

/**
 * @file
 * @brief URL短縮API
 */

/**
 * @brief URL短縮器インスタンス生成
 * @param[in]  capacity   キャッシュの容量（必須）
 * @param[out] out_us     生成されたインスタンスへのポインタ
 * @return                エラーコード
 */
ds_error_t ds_url_shortener_create(size_t capacity, ds_url_shortener_t** out_us);

/**
 * @brief URL短縮器破棄
 * @param us インスタンス
 * @return   エラーコード
 */
ds_error_t ds_url_shortener_destroy(ds_url_shortener_t* us);

/**
 * @brief 長いURLを短縮
 * @param us         インスタンス
 * @param url        元のURL（NULL終端文字列）
 * @param out_short  短縮URL出力バッファ
 * @param out_size   out_shortのバイト数
 * @return           エラーコード
 */
ds_error_t ds_url_shortener_shorten(ds_url_shortener_t* us, const char* url, char* out_short, size_t out_size);

/**
 * @brief 短縮URLを元URLへ展開
 * @param us         インスタンス
 * @param short_url  短縮URL（NULL終端文字列）
 * @param out_url    展開後のURL出力バッファ
 * @param out_size   out_urlのバイト数
 * @return           エラーコード
 */
ds_error_t ds_url_shortener_expand(ds_url_shortener_t* us, const char* short_url, char* out_url, size_t out_size);

#ifdef __cplusplus
}
#endif

#endif // DS_URL_SHORTENER_H