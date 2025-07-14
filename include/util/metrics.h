#ifndef UTIL_METRICS_H
#define UTIL_METRICS_H

#include "data_structures.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- 構造体本体は.cファイルに隠蔽 ---
typedef struct ds_metrics ds_metrics_t;

// --- 個別メトリクス構造体API ---
ds_error_t ds_metrics_init(ds_metrics_t *metrics);
ds_error_t ds_metrics_increment_ops(ds_metrics_t *metrics);
ds_error_t ds_metrics_increment_elements(ds_metrics_t *metrics);
ds_error_t ds_metrics_decrement_elements(ds_metrics_t *metrics);
ds_error_t ds_metrics_add_memory(ds_metrics_t *metrics, size_t bytes);
ds_error_t ds_metrics_sub_memory(ds_metrics_t *metrics, size_t bytes);
void       ds_metrics_print(const ds_metrics_t *metrics);
void       ds_metrics_reset_all(void);

// --- 🆕 ネームドカウンタAPI（global/labelベース型） ---
/**
 * @brief ラベル名付きカウンタをインクリメント
 * @param name カウンタ名（例："request", "hit", "miss", "test.counter"）
 */
void    ds_metrics_increment(const char *name);

/**
 * @brief ラベル名付きカウンタの現在値を取得
 * @param name カウンタ名
 * @return 現在値（存在しなければ0）
 */
int64_t ds_metrics_get(const char *name);

#ifdef __cplusplus
}
#endif

#endif // UTIL_METRICS_H