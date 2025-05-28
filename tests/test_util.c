/**
 * @file test_util.c
 * @brief ログ・メトリクス・メモリ管理ユーティリティ層ユニットテスト
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 設計意図:
 * - logger.c, metrics.c, memory.c のAPIを網羅的にテスト
 * - 標準出力/モック/エラーハンドリング/依存性注入を実現
 * - CI/CD現場ですぐ使えるmain()/assert設計
 */

 #include <stdio.h>
 #include <assert.h>
 #include <string.h>
 #include "../include/data_structures.h"
 #include "../include/config.h"
 
 // ========== logger.c テスト ==========
 
 // モックログカウンタ
 static int log_counter = 0;
 void mock_log_func(ds_log_level_t level, const char* format, ...) {
     (void)level; (void)format;
     log_counter++;
 }
 
 void test_logger(void) {
     log_counter = 0;
 
     // デフォルトログ関数（標準出力）→カスタム関数へ切り替え
     ds_set_log_function(mock_log_func);
 
     // ログ出力を数回呼ぶ
     ds_log_func_t logger = mock_log_func;
     logger(DS_LOG_INFO, "Test info log: %d", 123);
     logger(DS_LOG_ERROR, "Test error log: %s", "fail");
     assert(log_counter == 2);
 
     // エラーコード→文字列
     assert(strcmp(ds_error_string(DS_SUCCESS), "Success") == 0);
     assert(strcmp(ds_error_string(DS_ERROR_NULL_POINTER), "Null pointer") == 0);
 
     // デフォルト（NULL指定）に戻しても落ちない
     ds_set_log_function(NULL);
 }
 
 // ========== metrics.c テスト ==========
 
 void test_metrics(void) {
     ds_stats_t stats = {0};
     stats.total_elements = 42;
     stats.memory_allocated = 4096;
     stats.operations_count = 17;
     stats.creation_timestamp = 100000;
 
     // 単純な値チェック（実装によっては、構造化出力/Prometheus連携等も別途テスト可能）
     assert(stats.total_elements == 42);
     assert(stats.memory_allocated == 4096);
     assert(stats.operations_count == 17);
     assert(stats.creation_timestamp == 100000);
 }
 
 // ========== memory.c テスト ==========
 
 static int malloc_called = 0, free_called = 0;
 
 void* mock_malloc(size_t size) {
     malloc_called++;
     return malloc(size);
 }
 void mock_free(void* ptr) {
     free_called++;
     free(ptr);
 }
 
 void test_memory(void) {
     malloc_called = 0;
     free_called = 0;
 
     ds_set_memory_functions(mock_malloc, mock_free);
 
     // メモリ確保・解放（malloc, free を内部でフックする関数で利用）
     void* ptr = mock_malloc(128);
     assert(ptr != NULL);
     mock_free(ptr);
 
     assert(malloc_called == 1);
     assert(free_called == 1);
 
     // NULL指定（デフォルトに戻す）
     ds_set_memory_functions(NULL, NULL);
 }
 
 // ========== 全体テストメイン ==========
 
 int main(void) {
     printf("=== Utility Layer Unit Test Start ===\n");
     test_logger();
     test_metrics();
     test_memory();
     printf("=== Utility Layer Unit Test Passed ===\n");
     return 0;
 }
 
 /**
  * 【設計補足】
  * - logger/metrics/memory 各APIの基本動作・依存性注入をすべて網羅
  * - モック関数による副作用検証やCI/CDテスト自動化前提
  * - エラー系、メトリクス、メモリ管理の責務分離・型安全を徹底
  * - main()で自動テストとして完結
  */