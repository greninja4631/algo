/**
 * @file hashmap.h
 * @brief HashMap ADT interface with chaining collision handling (シンプルなチェイニング実装)
 * 
 * - 責務：汎用ハッシュテーブルAPI
 * - 型安全・再利用性・エラー定義
 */

 #ifndef HASHMAP_H
 #define HASHMAP_H
 
 #include <stddef.h>
 #include <stdbool.h>
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /// エラー種別
 typedef enum {
     HM_OK = 0,
     HM_ERR_ALLOC,
     HM_ERR_KEY_NOT_FOUND,
     HM_ERR_DUPLICATE_KEY,
     HM_ERR_NULL,
     HM_ERR_OTHER
 } hashmap_error_t;
 
 /// Key/Value型
 typedef struct hashmap hashmap_t;
 
 /// コールバック（メモリ管理のため、ユーザー提供）
 typedef void (*hm_free_func)(void*);
 
 /// API設計（ADT: インターフェースのみ）
 hashmap_t* hashmap_create(size_t capacity, hm_free_func key_free, hm_free_func val_free);
 void hashmap_destroy(hashmap_t* map);
 
 hashmap_error_t hashmap_put(hashmap_t* map, const char* key, void* value);
 hashmap_error_t hashmap_get(hashmap_t* map, const char* key, void** out_value);
 hashmap_error_t hashmap_remove(hashmap_t* map, const char* key);
 
 size_t hashmap_size(const hashmap_t* map);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif // HASHMAP_H