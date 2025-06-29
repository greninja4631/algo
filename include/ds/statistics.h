#ifndef DS_STATISTICS_H
#define DS_STATISTICS_H

#include <stddef.h>  // size_t 型のため

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file statistics.h
 * @brief 整数配列に対する統計量計算（合計・最小・最大・平均・中央値・最頻値）
 * @author Your Name
 * @date   2025-06-27
 *
 * - 汎用統計構造体（ds_statistics_t）
 * - 統計量一括計算API（ds_calculate_statistics）
 */

/**
 * @struct ds_statistics_t
 * @brief 統計量の結果を格納する構造体
 */
typedef struct {
    int    sum;      ///< 合計値
    int    min;      ///< 最小値
    int    max;      ///< 最大値
    double average;  ///< 平均値
    double median;   ///< 中央値
    int    mode;     ///< 最頻値
} ds_statistics_t;

/**
 * @brief 整数配列の統計量を計算
 * @param data 整数配列（NULL不可）
 * @param size 配列サイズ（0不可）
 * @return 統計結果（ds_statistics_t型、全フィールド0なら異常）
 */
ds_statistics_t ds_calculate_statistics(const int *data, size_t size);

#ifdef __cplusplus
}
#endif

#endif // DS_STATISTICS_H
