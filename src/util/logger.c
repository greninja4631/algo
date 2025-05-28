/**
 * @file logger.c
 * @brief シンプルかつクラウド観測性を意識した構造化ログユーティリティ
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 【設計意図】
 * - ログ出力をアプリの各所から一元的に扱うことで保守性/再利用性/観測性を向上
 * - 標準出力/標準エラー出力だけでなくクラウド/テスト環境にも差し替えやすい構造
 * - DevOpsのCI/CD、GCP/AWS/Datadog等のクラウド監視基盤との連携も視野
 * - 依存性注入（DI）によるテスト容易性
 */

 #include "data_structures.h"
 #include <stdio.h>
 #include <stdarg.h>
 #include <time.h>
 
 /* --- グローバル状態：ログ出力関数ポインタ（依存性注入/DI対応） --- */
 static ds_log_func_t custom_logger = NULL;
 
 /**
  * @brief 標準ログ出力の実装（デフォルト）
  * - 本番: stdout, エラー: stderr
  * - 必要に応じてJSON構造化ログにも拡張可能
  */
 static void default_log_func(ds_log_level_t level, const char* format, ...) {
     va_list args;
     FILE* out = (level >= DS_LOG_ERROR) ? stderr : stdout;
 
     // タイムスタンプ出力（ISO 8601形式）
     time_t now = time(NULL);
     struct tm tm_info;
     localtime_r(&now, &tm_info);
     char buf[32];
     strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm_info);
 
     // ログレベル文字列
     const char* level_str = "";
     switch (level) {
         case DS_LOG_DEBUG: level_str = "DEBUG"; break;
         case DS_LOG_INFO:  level_str = "INFO "; break;
         case DS_LOG_WARN:  level_str = "WARN "; break;
         case DS_LOG_ERROR: level_str = "ERROR"; break;
         case DS_LOG_FATAL: level_str = "FATAL"; break;
         default:           level_str = "LOG  "; break;
     }
 
     // フォーマット: [時刻][レベル] メッセージ
     fprintf(out, "[%s][%s] ", buf, level_str);
     va_start(args, format);
     vfprintf(out, format, args);
     va_end(args);
     fprintf(out, "\n");
     fflush(out); // ログが即座に出るようにflush
 }
 
 /**
  * @brief ログ出力関数のセット（依存性注入/テスト差し替え用）
  * - NULLを渡すとデフォルト実装になる
  */
 void ds_set_log_function(ds_log_func_t log_func) {
     custom_logger = log_func;
 }
 
 /**
  * @brief ログを出力するユーティリティ関数
  * - API/アプリ内部はこの関数を直接使うことで、どこでもログ一元化
  * - モックやクラウド実装と容易に切り替え可
  */
 void ds_log(ds_log_level_t level, const char* format, ...) {
     va_list args;
     va_start(args, format);
     if (custom_logger) {
         // 可変長引数を直接渡せないので一旦vprintf的に展開
         // ⇒推奨：ラップ用のvlog関数も実装するとよい
         // ここでは一時バッファで実装
         char tmp[1024];
         vsnprintf(tmp, sizeof(tmp), format, args);
         custom_logger(level, "%s", tmp);
     } else {
         // デフォルト（標準出力/エラー出力）
         va_list args2;
         va_start(args2, format);
         default_log_func(level, format, args2);
         va_end(args2);
     }
     va_end(args);
 }
 
 /**
  * @brief ログレベルごとの簡易API（利便性UP）
  * - 例: ds_log_info("Process started: %d", pid);
  */
 void ds_log_debug(const char* format, ...) {
     va_list args;
     va_start(args, format);
     ds_log(DS_LOG_DEBUG, format, args);
     va_end(args);
 }
 void ds_log_info(const char* format, ...) {
     va_list args;
     va_start(args, format);
     ds_log(DS_LOG_INFO, format, args);
     va_end(args);
 }
 void ds_log_warn(const char* format, ...) {
     va_list args;
     va_start(args, format);
     ds_log(DS_LOG_WARN, format, args);
     va_end(args);
 }
 void ds_log_error(const char* format, ...) {
     va_list args;
     va_start(args, format);
     ds_log(DS_LOG_ERROR, format, args);
     va_end(args);
 }
 void ds_log_fatal(const char* format, ...) {
     va_list args;
     va_start(args, format);
     ds_log(DS_LOG_FATAL, format, args);
     va_end(args);
 }
 
 /* --- テスト容易性/CI/CD運用 --- */
 /*
     - logger.cの各APIはmain()依存無し・グローバル状態極小
     - ユニットテストやクラウド用実装に容易に差し替え可
     - 標準出力だけでなくCloud LoggingやDatadogのAPI呼び出しに切り替えられる
     - 本番運用ではJSON形式/マスク処理/トレースID付与などを追加可能
 */