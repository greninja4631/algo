<<<<<<< HEAD
#ifndef URL_SHORTENER_H
#define URL_SHORTENER_H

#include <stddef.h>
#include <stdbool.h>
#include "../data_structures.h"// ds_error_t型など共通定義
=======
#ifndef DS_URL_SHORTENER_H
#define DS_URL_SHORTENER_H

#include "data_structures.h"  // ds_url_shortener_t, ds_error_t など
>>>>>>> feature

#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD
typedef struct ds_url_shortener ds_url_shortener_t;

// URL短縮器の生成・破棄
ds_url_shortener_t* ds_url_shortener_create(void);
ds_error_t ds_url_shortener_destroy(ds_url_shortener_t* us);

// URLの短縮（短縮URL生成）
// out_short: 生成された短縮URLを書き込むバッファ, out_size: バッファサイズ
ds_error_t ds_url_shortener_shorten(ds_url_shortener_t* us, const char* url, char* out_short, size_t out_size);

// 短縮URLから元URLへの展開
=======
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
ds_error_t ds_url_shortener_create(const ds_allocator_t* alloc, size_t capacity, ds_url_shortener_t** out_us);

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
>>>>>>> feature
ds_error_t ds_url_shortener_expand(ds_url_shortener_t* us, const char* short_url, char* out_url, size_t out_size);

#ifdef __cplusplus
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#endif // URL_SHORTENER_H
=======
#endif // DS_URL_SHORTENER_H
>>>>>>> feature
=======
#endif // DS_URL_SHORTENER_H
>>>>>>> feature
