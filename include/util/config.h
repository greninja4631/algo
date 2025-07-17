/**
 * @file    util/config.h
 * @brief   ライブラリ共通設定API（Opaque型 + DIアロケータ対応）
 * @details 本体定義は src/util/config.c 限定、API宣言のみ
 */
#ifndef DS_CONFIG_H
#define DS_CONFIG_H

#include "data_structures.h"   // ds_error_t, ds_allocator_t, ds_config_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 設定ハンドル生成
 * @param[in]  alloc    抽象アロケータ（DI対応, NULL禁止）
 * @param[out] out_cfg  生成されたハンドル（NULL時はエラー）
 * @return DS_SUCCESS / DS_ERR_ALLOC / DS_ERR_NULL_POINTER
 * @ownership caller frees (ds_config_destroy)
 */
ds_error_t ds_config_create(const ds_allocator_t* alloc, ds_config_t **out_cfg);

/**
 * @brief ハンドル破棄（NULL安全）
 * @param[in]  alloc   抽象アロケータ（DI対応, NULL許容）
 * @param[in]  cfg     破棄するハンドル（NULL許容）
 * @ownership caller frees
 */
void ds_config_destroy(const ds_allocator_t* alloc, ds_config_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* DS_CONFIG_H */
