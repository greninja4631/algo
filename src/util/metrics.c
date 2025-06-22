/**
 * @file memory.c
 * @brief メモリ管理の依存性注入と監視拡張ポイント（クラウド/テスト/モック/高速化用）
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 【設計理念】
 * - malloc/freeを直接呼ぶのではなく、差し替え可能な関数経由で全メモリアクセスを統一
 * - メモリリーク検出、GC、クラウド運用・テスト用の模擬メモリ障害対応
 * - 監視や高速アロケータ(jemalloc/tcmalloc)等への将来拡張
 * - シリコンバレー標準の「依存性注入」設計
 */

 #include "data_structures.h"
 #include <stdlib.h>
 #include <stdio.h>
 
 /* ---- グローバル状態：アロケータ関数ポインタ ---- */
 static ds_malloc_func_t custom_malloc = NULL;
 static ds_free_func_t custom_free = NULL;
 
 /**
  * @brief メモリ関数の登録（依存性注入。NULLで標準関数に戻る）
  * @param malloc_func malloc代替関数
  * @param free_func free代替関数
  */
 void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
     custom_malloc = malloc_func;
     custom_free   = free_func;
 }
 
 /**
  * @brief メモリ確保ラッパー
  * - テストやクラウド用に差し替え可能
  * - 将来のトラッキングや高速化に備え一元管理
  */
 void* ds_malloc(size_t size) {
     if (custom_malloc)
         return custom_malloc(size);
     return malloc(size);
 }
 
 /**
  * @brief メモリ解放ラッパー
  * - テストやGC、クラウド拡張に対応
  */
 void ds_free(void* ptr) {
     if (custom_free)
         custom_free(ptr);
     else
         free(ptr);
 }
 
 /**
  * @brief メモリ確保/解放時にメトリクスを更新する例（オプション）
  * - metrics.cのAPIと組み合わせてメモリ使用量のリアルタイム監視が可能
  */
 
 // 例: カスタムmallocでアロケート状況を監視したい場合
 /*
 static void* tracked_malloc(size_t size) {
     void* p = malloc(size);
     if (p) ds_metrics_update(global_stats, 0, 0, size); // グローバルなメトリクス等
     return p;
 }
 static void tracked_free(void* ptr) {
     // サイズを記録しておく場合のみ。ここでは例示のみ。
     ds_metrics_update(global_stats, 0, 0, -prev_size);
     free(ptr);
 }
 */
 
 /* --- テスト容易性/CI/CD運用 ---
     - CIではメモリ障害模擬やvalgrind、GC実装実験等も差し替えのみで実現可
     - 依存性注入により本番/テスト/開発の最適化が容易
 */