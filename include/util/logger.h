<<<<<<< HEAD
#ifndef DS_LOGGER_H
#define DS_LOGGER_H
#define DS_LOG_ERROR DS_LOG_LEVEL_ERROR


<<<<<<< HEAD
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

// ログレベル
typedef enum {
    DS_LOG_INFO,
    DS_LOG_WARN,
    DS_LOG_ERROR,
    DS_LOG_DEBUG
} ds_log_level_t;

// ログ関数型
typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...);

// グローバルログ関数の設定
void ds_set_log_function(ds_log_func_t func);

// デフォルトログ（標準出力）
void ds_log(ds_log_level_t level, const char* fmt, ...);

// ショートカットマクロ
#define DS_LOG_INFO  DS_LOG_INFO
#define DS_LOG_ERROR DS_LOG_ERROR

// エラーコードと変換
typedef enum {
    DS_SUCCESS = 0,
    DS_ERR_NULL_POINTER,
    DS_ERR_ALLOC,
    // ... 他のエラー
} ds_error_t;

const char* ds_error_string(ds_error_t err);

=======
#include <stdarg.h>

=======
>>>>>>> feature
/**
 * @file   util/logger.h
 * @brief  共通ロガー API ― 全モジュールで共有
 *
 * ▼ 2025-07 Cプロジェクト APIガイドライン適合ポイント
 *   • enum 名は必ず DS_LOG_LEVEL_* で統一し、値を明示
 *   • エラー処理と同じ “DI” 方針：外部からロガー実装を差し替え可能
 *   • printf 直接使用禁止 ──> すべて ds_log() 経由
 *   • Opaque 型は不要なのでヘッダに実装は置かない
 */

#ifndef DS_UTIL_LOGGER_H          /* <include guard> */
#define DS_UTIL_LOGGER_H

#include "data_structures.h"      /* プロジェクト共通型 */
#include <stdarg.h>               /* va_list            */

#ifdef __cplusplus
extern "C" {
#endif

/*──────────────────────────────────────
 * ログレベル（値が小さいほど重大）
 *──────────────────────────────────────*/
typedef enum {
    DS_LOG_LEVEL_ERROR = 0,   /* 重大なエラー ― 直ちに対処が必要 */
    DS_LOG_LEVEL_WARN,        /* 警告 ― 動作は継続するが要注意  */
    DS_LOG_LEVEL_INFO,        /* 情報 ― 通常運転の状態遷移など    */
    DS_LOG_LEVEL_DEBUG,       /* デバッグ ― 詳細トレース          */
    DS_LOG_LEVEL_FATAL        /* 致命的 ― 強制終了前最後のログ   */
} ds_log_level_t;

/* 後方互換マクロ（過去コードに配慮） */
#define DS_LOG_ERROR DS_LOG_LEVEL_ERROR

/*──────────────────────────────────────
 * DI 用ロガー関数ポインタ
 *──────────────────────────────────────*/
typedef void (*ds_log_func_t)(ds_log_level_t level,
                              const char    *fmt,
                              va_list        args);

/**
 * @brief ロガー実装を差し替える
 * @param func  printf 互換のロガー。NULL でデフォルトに戻す
 * @note  スレッドセーフ保証は呼び出し側で行うこと
 */
void ds_set_log_function(ds_log_func_t func);

/**
 * @brief  共通ログAPI（可変長引数）
 * @param  level  DS_LOG_LEVEL_*
 * @param  fmt    printf 互換フォーマット
 */
void ds_log(ds_log_level_t level, const char *fmt, ...);

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature
#endif // LOGGER_H
=======


#endif /* DS_LOGGER_H */
<<<<<<< HEAD
>>>>>>> feature
=======
>>>>>>> feature
=======
#ifdef __cplusplus
}
#endif
#endif /* DS_UTIL_LOGGER_H */
>>>>>>> feature
