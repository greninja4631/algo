<<<<<<< HEAD
<<<<<<< HEAD
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
=======
//statistics.hの役割は、statistics.hで関数の挙動を呼び起こした時に、statistics.h経由で、statistics.cを呼び起こす方法に一貫性を持たせることが目的である。
=======
#ifndef DS_STATISTICS_H
#define DS_STATISTICS_H
>>>>>>> feature

#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

ds_error_t ds_statistics_create(const ds_allocator_t* alloc, ds_statistics_t** out_stats);
void       ds_statistics_destroy(const ds_allocator_t* alloc, ds_statistics_t* stats);
ds_error_t ds_statistics_calculate(ds_statistics_t* stats, const int* data, size_t size);

ds_error_t ds_statistics_get_sum    (const ds_statistics_t* stats, int* out_sum);
ds_error_t ds_statistics_get_min    (const ds_statistics_t* stats, int* out_min);
ds_error_t ds_statistics_get_max    (const ds_statistics_t* stats, int* out_max);
ds_error_t ds_statistics_get_average(const ds_statistics_t* stats, double* out_avg);
ds_error_t ds_statistics_get_median (const ds_statistics_t* stats, double* out_median);
ds_error_t ds_statistics_get_mode   (const ds_statistics_t* stats, int* out_mode);

ds_error_t ds_statistics_calculate_all(
    const int* data, size_t size,
    int* out_sum, int* out_min, int* out_max,
    double* out_avg, double* out_median, int* out_mode
);

#ifdef __cplusplus
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#endif // STATISTICS_H
=======
#endif // STATISTICS_H

//成功しました
>>>>>>> feature
>>>>>>> feature
=======
#endif
>>>>>>> feature
