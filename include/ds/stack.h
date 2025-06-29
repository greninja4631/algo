#ifndef DS_STACK_H
#define DS_STACK_H

#include <stddef.h>
#include <stdbool.h>
#include "../data_structures.h"// 共通エラー型・統計型・共通アロケータなど

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ds_stack ds_stack_t;

/**
 * @brief スタック生成
 * @return ds_stack_t* スタックのポインタ
 */
ds_stack_t* ds_stack_create(void);

/**
 * @brief スタック破棄
 * @param stack 破棄対象
 * @return エラーコード
 */
ds_error_t ds_stack_destroy(ds_stack_t* stack);

/**
 * @brief プッシュ（データ追加）
 * @param stack スタック
 * @param data 追加データ
 * @return エラーコード
 */
ds_error_t ds_stack_push(ds_stack_t* stack, void* data);

/**
 * @brief ポップ（データ取り出し＆削除）
 * @param stack スタック
 * @param out_data 取得先
 * @return エラーコード
 */
ds_error_t ds_stack_pop(ds_stack_t* stack, void** out_data);

/**
 * @brief ピーク（先頭データ取得、削除はしない）
 * @param stack スタック
 * @param out_data 取得先
 * @return エラーコード
 */
ds_error_t ds_stack_peek(const ds_stack_t* stack, void** out_data);

/**
 * @brief スタック空判定
 */
bool ds_stack_is_empty(const ds_stack_t* stack);

/**
 * @brief スタックのサイズ
 */
size_t ds_stack_size(const ds_stack_t* stack);

/**
 * @brief スタックの全データリセット（メモリは解放しない）
 */
ds_error_t ds_stack_reset(ds_stack_t* stack);

/**
 * @brief 統計情報取得
 */
ds_error_t ds_stack_get_stats(const ds_stack_t* stack, ds_stats_t* stats);

#ifdef __cplusplus
}
#endif

#endif // DS_STACK_H
