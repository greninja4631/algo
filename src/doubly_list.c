#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stddef.h> // size_t のため
#include <stdbool.h> // bool のため (C23以前でキーワードでない場合)
#include <stdarg.h> // va_list, va_start, va_end のため

/* --- エラーコード --- */
typedef enum {
    DS_SUCCESS = 0,         // 処理成功
    DS_ERR_NULL_POINTER,    // NULLポインタ引数
    DS_ERR_ALLOC,           // メモリ割り当て失敗
    DS_ERR_OUT_OF_RANGE,    // インデックスまたは引数が有効範囲外
    DS_ERR_EMPTY,           // 空の構造体に対する操作
    DS_ERR_NOT_FOUND,       // アイテムが見つからない
    DS_ERR_INVALID_ARG,     // 無効な引数
    DS_ERR_IO,              // 入出力エラー
    DS_ERR_UNKNOWN          // 不明なエラーまたは一般エラー
} ds_error_t;

/* --- ログレベル (ソースファイルで使用されているもの) --- */
typedef enum {
    DS_LOG_DEBUG,
    DS_LOG_INFO,
    DS_LOG_WARNING,
    DS_LOG_ERROR,
    DS_LOG_FATAL
} ds_log_level_t;

/* --- 関数ポインタ型 (ソースファイルで使用されているもの) --- */
// メモリ管理用
typedef void* (*ds_malloc_func_t)(size_t size);
typedef void  (*ds_free_func_t)(void* ptr);

// ロギング用
typedef void (*ds_log_func_t)(ds_log_level_t level, const char* fmt, ...);

/* --- 双方向リストの不透明型宣言 --- */
// これにより、ライブラリの利用者は ds_doubly_list_t の内部構造を知ることなく
// ds_doubly_list_t へのポインタを宣言できます。
typedef struct ds_doubly_list ds_doubly_list_t;


/* ---------------------------
 * 公開API宣言 (ソースファイルと一致させること)
 * -------------------------- */

ds_doubly_list_t* ds_doubly_list_create(void);
ds_error_t ds_doubly_list_destroy(ds_doubly_list_t* list);
ds_error_t ds_doubly_list_insert_front(ds_doubly_list_t* list, void* data);
ds_error_t ds_doubly_list_insert_back(ds_doubly_list_t* list, void* data);
ds_error_t ds_doubly_list_insert_at(ds_doubly_list_t* list, size_t index, void* data);
ds_error_t ds_doubly_list_remove_front(ds_doubly_list_t* list, void** data);
ds_error_t ds_doubly_list_remove_back(ds_doubly_list_t* list, void** data);
ds_error_t ds_doubly_list_remove_at(ds_doubly_list_t* list, size_t index, void** data);
ds_error_t ds_doubly_list_get_at(const ds_doubly_list_t* list, size_t index, void** data);
bool ds_doubly_list_is_empty(const ds_doubly_list_t* list);
size_t ds_doubly_list_size(const ds_doubly_list_t* list);
void ds_set_log_function(ds_log_func_t func);
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func);

#endif // DATA_STRUCTURES_H