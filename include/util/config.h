/**
 * @file    include/util/config.h
 * @brief   ライブラリ共通設定（Opaque ハンドル）
 *
 * ※ 実装は src/util/config.c に隠蔽。ここでは **宣言だけ** を行う。
 */
#ifndef DS_CONFIG_H            /* ← ガイドライン準拠ガードに統一 */
#define DS_CONFIG_H

#include "data_structures.h"   /* ds_error_t 等を利用 */

#ifdef __cplusplus
extern "C" {
#endif

/*─── Opaque 型 ───*/
typedef struct ds_config ds_config_t;

/*─── API ───*/
/**
 * @brief 設定ハンドル生成
 * @param[out] out_cfg 生成されたハンドル
 * @return DS_SUCCESS / DS_ERR_ALLOC / DS_ERR_NULL_POINTER
 * @ownership caller frees
 */
ds_error_t ds_config_create(ds_config_t **out_cfg);

/** 
 * @brief ハンドル破棄（NULL 安全） 
 * @param cfg 破棄するハンドル（NULL 安全）
 */
void ds_config_destroy(ds_config_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* DS_CONFIG_H */