#ifndef URL_SHORTENER_H
#define URL_SHORTENER_H

#include <stddef.h>
#include <stdbool.h>
#include "../data_structures.h"// ds_error_t型など共通定義

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_url_shortener ds_url_shortener_t;

// URL短縮器の生成・破棄
ds_url_shortener_t* ds_url_shortener_create(void);
ds_error_t ds_url_shortener_destroy(ds_url_shortener_t* us);

// URLの短縮（短縮URL生成）
// out_short: 生成された短縮URLを書き込むバッファ, out_size: バッファサイズ
ds_error_t ds_url_shortener_shorten(ds_url_shortener_t* us, const char* url, char* out_short, size_t out_size);

// 短縮URLから元URLへの展開
ds_error_t ds_url_shortener_expand(ds_url_shortener_t* us, const char* short_url, char* out_url, size_t out_size);

#ifdef __cplusplus
}
#endif

#endif // URL_SHORTENER_H
