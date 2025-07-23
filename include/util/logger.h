/**
 * @file   util/logger.h
 * @brief  全モジュール共通ロガー API (APIプロトタイプ専用、typedef一切禁止)
 *
 * ▼ 2025-07 Cプロジェクト APIガイドライン
 *   • typedef/enum/前方宣言は "必ず" data_structures.h だけに集約
 *   • 本ヘッダには関数宣言・マクロのみ
 *   • printf/puts直書き禁止・必ずds_log()経由
 *   • DI（差し替え）API提供
 */

#ifndef DS_UTIL_LOGGER_H
#define DS_UTIL_LOGGER_H

#include "data_structures.h"  /* 共通型（ds_log_level_t, ds_log_func_t） */

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/*** 本ヘッダには"typedef/enum/struct宣言は一切書かない"！ ***/

/**
 * @brief ロガー関数のDI差し替え
 * @param func   printf互換、NULLでデフォルト
 * @note   スレッドセーフは呼び出し側責任
 */
void ds_set_log_function(ds_log_func_t func);

/**
 * @brief 共通ログAPI
 * @param level DS_LOG_LEVEL_*
 * @param fmt   printf互換
 */
void ds_log(ds_log_level_t level, const char *fmt, ...);

#ifdef __cplusplus
}
#endif
#endif /* DS_UTIL_LOGGER_H */
