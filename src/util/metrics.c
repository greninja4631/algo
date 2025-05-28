/**
 * @file metrics.c
 * @brief データ構造ライブラリ用クラウド対応メトリクス収集モジュール
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 【設計理念】
 * - ライブラリ内部の状態変化や操作回数を集計し、外部から容易に観測できる設計
 * - Prometheus/GCP Monitoring/Datadogなどのクラウド監視連携を意識
 * - 依存性注入・テスト容易性・将来の拡張性（新規メトリクス追加等）を重視
 */

 #include "data_structures.h"
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 
 /* ---- グローバルメトリクス管理 ----
  * ここでは最小限のメトリクス（操作回数/メモリ使用量等）を実装。
  * 本番では各データ構造インスタンスごとに個別管理することを推奨。
  */
 
 /**
  * @brief メトリクスを初期化するユーティリティ関数
  * @param stats 初期化するメトリクス構造体
  */
 void ds_metrics_init(ds_stats_t* stats) {
     if (!stats) return;
     stats->total_elements    = 0;
     stats->memory_allocated  = 0;
     stats->operations_count  = 0;
     stats->creation_timestamp = (uint64_t)time(NULL);
 }
 
 /**
  * @brief 要素数・操作回数などの値を更新する関数
  * @param stats 対象メトリクス
  * @param delta_elements 要素数の増減値
  * @param delta_ops 操作回数の増分
  * @param memory_change 追加・削除メモリ量
  */
 void ds_metrics_update(ds_stats_t* stats, ssize_t delta_elements, size_t delta_ops, ssize_t memory_change) {
     if (!stats) return;
     if (delta_elements < 0 && (size_t)(-delta_elements) > stats->total_elements)
         stats->total_elements = 0;
     else
         stats->total_elements += delta_elements;
 
     stats->operations_count += delta_ops;
 
     if (memory_change < 0 && (size_t)(-memory_change) > stats->memory_allocated)
         stats->memory_allocated = 0;
     else
         stats->memory_allocated += memory_change;
 }
 
 /**
  * @brief メトリクス情報のコピー取得
  * @param src コピー元
  * @param dest コピー先
  */
 void ds_metrics_copy(const ds_stats_t* src, ds_stats_t* dest) {
     if (!src || !dest) return;
     memcpy(dest, src, sizeof(ds_stats_t));
 }
 
 /**
  * @brief 監視ツール等へのメトリクス出力 (拡張ポイント)
  * - 本格運用ではここからPrometheus/GCP/Datadog等APIへ連携可
  * @param stats 出力するメトリクス
  */
 void ds_metrics_export(const ds_stats_t* stats) {
     if (!stats) return;
     // 現状は標準出力。将来的にはJSONやクラウドAPI出力等に拡張可
     printf("{\"elements\":%zu,\"memory\":%zu,\"ops\":%zu,\"created\":%llu}\n",
         stats->total_elements, stats->memory_allocated, stats->operations_count,
         (unsigned long long)stats->creation_timestamp);
 }
 
 /* --- テスト容易性/CI/CD運用 ---
     - ds_metrics_init()/update()/copy() などはmain依存無し
     - モック/クラウド拡張容易、CIでカバレッジ向上
 */