/**
 * @file config.h
 * @brief 設定管理API ― 設定構造体・生成/破棄関数の宣言（本体は.cに隠蔽）
 * @version 1.0.0
 * @date 2025-07-11
 *
 * 設計指針:
 * - 利用者はこのヘッダからAPIのみ知る（本体は絶対非公開/O-DRY原則）
 * - グローバル値・マジックナンバー等は別途「ds_global_config.h」へ分離推奨
 */
#ifndef UTIL_CONFIG_H
#define UTIL_CONFIG_H


#ifdef __cplusplus
extern "C" {
#endif

/** 設定構造体の生成 */
ds_config_t* ds_config_create(void);

/** 設定構造体の破棄 */
void ds_config_destroy(ds_config_t* config);

#ifdef __cplusplus
}
#endif

#endif // UTIL_CONFIG_H