/**
 * @file lru_cache.h
 * @brief ジェネリックvoid*値対応 LRUキャッシュAPI（HashMap+双方向リスト合成型）
 * @details
 *   - DSルール：関数名・型名「ds_」で統一
 *   - 内部でds_malloc/ds_free/ds_logを使用
 *   - capacity: キャッシュ最大要素数（超過時は自動削除）
 */

#ifndef DS_LRU_CACHE_H
#define DS_LRU_CACHE_H

#include "../data_structures.h"
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 不透明型
typedef struct ds_lru_cache ds_lru_cache_t;

/**
 * @brief LRUキャッシュ生成
 * @param capacity 最大格納数
 * @return ポインタ or NULL
 */
ds_lru_cache_t* ds_lru_cache_create(size_t capacity);

/**
 * @brief キャッシュ破棄
 */
ds_error_t ds_lru_cache_destroy(ds_lru_cache_t* cache);

/**
 * @brief put: キーに値追加/上書き
 * @param cache キャッシュ
 * @param key int型キー（必要なら文字列keyも設計可能）
 * @param value ポインタ（void*）
 * @return エラーコード
 */
ds_error_t ds_lru_cache_put(ds_lru_cache_t* cache, int key, void* value);

/**
 * @brief get: キーで値取得（アクセスで自動的に最前列へ移動）
 * @param cache キャッシュ
 * @param key int型キー
 * @param value 見つからなければ*value==NULL
 * @return エラーコード
 */
ds_error_t ds_lru_cache_get(ds_lru_cache_t* cache, int key, void** value);

/**
 * @brief サイズ取得
 */
size_t ds_lru_cache_size(const ds_lru_cache_t* cache);

#ifdef __cplusplus
}
#endif

#endif // DS_LRU_CACHE_H
