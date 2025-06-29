// src/util/logger.c
#include "../../include/data_structures.h"
#include <stdio.h>
#include <stdarg.h>

// ===== エラーコード → 文字列 =====
const char* ds_error_string(ds_error_t err) {
    switch (err) {
        case DS_SUCCESS:             return "Success";
        case DS_ERR_NULL_POINTER:    return "Null pointer";
        case DS_ERR_ALLOC:           return "Allocation error";
        case DS_ERR_EMPTY:           return "Empty";
        case DS_ERR_INVALID_ARG:     return "Invalid argument";
        case DS_ERR_OVERFLOW:        return "Overflow";
        case DS_ERR_UNDERFLOW:       return "Underflow";
        case DS_ERR_NOT_FOUND:       return "Not found";
        case DS_ERR_SYSTEM:          return "System error";
        case DS_ERR_OUT_OF_MEMORY:   return "Out of memory";
        case DS_ERR_EMPTY_CONTAINER: return "Empty container";
        case DS_ERR_SYSTEM_FAILURE:  return "System failure";
        default:                     return "Unknown error";
    }
}

// ===== デフォルトロガー実装（stderrに出力） =====
static void default_log(ds_log_level_t level, const char* fmt, ...) {
    (void)level; // levelでログフィルタ等も今後拡張可能
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

// ===== グローバルなログ関数ポインタ =====
static ds_log_func_t g_log_func = NULL;

// ===== ログ関数の差し替えAPI =====
void ds_set_log_function(ds_log_func_t func) {
    g_log_func = func ? func : default_log;
}

// ===== ログ出力API（全ファイルはこれを使う） =====
void ds_log(ds_log_level_t level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (g_log_func)
    {
        // g_log_funcはva_list非対応なので、vfprintf同様に可変引数を直接渡す
        // 可変長引数をラップして渡す
        g_log_func(level, fmt, args);
    }
    va_end(args);
}

// ===== 初期化 =====
__attribute__((constructor))
static void logger_init(void) {
    // プログラム起動時にデフォルトのロガーをセット
    g_log_func = default_log;
}
#include "../../include/util/logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// ログレベル文字列
static const char* ds_log_level_str[] = {
    "INFO", "WARN", "ERROR"
};

// デフォルトのログ関数
static void default_log_func(ds_log_level_t level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[%s] ", ds_log_level_str[level]);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

// 現在のログ関数（初期値: デフォルト）
static ds_log_func_t g_log_func = default_log_func;

// ログ関数を差し替えるAPI
void ds_set_log_function(ds_log_func_t func)
{
    g_log_func = func ? func : default_log_func;
}

// ログ出力API本体
void ds_log(ds_log_level_t level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (g_log_func) {
        // 関数ポインタ渡し → vsnprintfではなく vfprintf互換で良い
        g_log_func(level, fmt, args);
    }
    va_end(args);
}

// エラーコード → 文字列
const char* ds_error_string(ds_error_t err)
{
    switch (err) {
        case DS_SUCCESS:            return "Success";
        case DS_ERR_NULL_POINTER:   return "Null pointer";
        case DS_ERR_ALLOC:          return "Allocation failed";
        case DS_ERR_OUT_OF_MEMORY:  return "Out of memory";
        case DS_ERR_SYSTEM_FAILURE: return "System failure";
        case DS_ERR_INVALID_ARG:    return "Invalid argument";
        case DS_ERR_EMPTY_CONTAINER:return "Empty container";
        case DS_ERR_NOT_FOUND:      return "Not found";
        default:                    return "Unknown error";
    }
}
