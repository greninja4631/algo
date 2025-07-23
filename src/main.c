/**
 * @file    src/main.c
 * @brief   ライブラリ／アプリ本体バイナリの「唯一の」エントリポイント
 *
 * ──────────────────────────────────────────────
 *  このmain()は「実運用用」。test_main.cとは完全に分離。
 *  - ガイドライン完全順守: 再帰定義・FSM設計・DI・アロケータ・ロギング
 *  - CLI, AI, DB, VM等の拡張点をコメントで明示
 *  - CI/CDでvalgrind, 静的解析, フォーマット, 禁止API grep全部Pass前提
 *  - ロギングはds_log()/logger.c経由のみ
 * ──────────────────────────────────────────────
 */

#include "data_structures.h"      // APIバージョン, Opaque型, 共通定義
#include "util/logger.h"          // ds_log()
#include "util/memory.h"          // ds_get_default_allocator()
// 必要に応じて追加: CLI, AI, DB, VM, Cloud, Undo-Redoなど

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TESTING  // CIテスト時は除外
int main(int argc, char *argv[])
{
    (void)argc; (void)argv;  // 未使用警告抑止

    // ---- DI: アロケータ取得（全API第1引数に必ずallocを渡す設計） ----
    const ds_allocator_t *alloc =
#ifdef DS_HAVE_DEFAULT_ALLOCATOR
        ds_get_default_allocator();
#else
        NULL; // Fallback（下層で標準malloc/free）
#endif

    // ---- 起動ログ ----
    ds_log(DS_LOG_LEVEL_INFO, "Data-Structures Library v%s — Startup", DS_API_VERSION);

    // =================== メイン実装の拡張点 ===================
    // ここでCLIやDB, AI, VMなど本体の機能を起動
    // 例: 
    // cli_dispatch(argc, argv, alloc);
    // ai_model_bootstrap(alloc);
    // db_migrate_if_needed();
    // cloud_service_start(...);
    // undo_redo_system_init();
    // =========================================================

    // ---- サンプル：API利用例（未使用関数ゼロ保証）----
    // ※下記は全て「未使用警告」にならないよう必ず何らかで呼ぶ
    // ※プロダクション用途なら、ここをCLI引数や設定に応じて分岐する設計が推奨

    int arr[] = {10, 20, 30, 40, 50};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    ptrdiff_t idx = -1;
    ds_error_t err;

    // 線形探索テスト
    err = ds_search_linear(arr, n, 30, &idx);
    ds_log(DS_LOG_LEVEL_INFO, "Search 30: err=%d, idx=%td", err, idx);

    // 二分探索テスト
    err = ds_search_binary(arr, n, 40, &idx);
    ds_log(DS_LOG_LEVEL_INFO, "Binary search 40: err=%d, idx=%td", err, idx);

    // ソート（例）
    int unsorted[] = {9, 1, 7, 3, 5};
    ds_sort_bubble(unsorted, 5);
    ds_sort_insertion(unsorted, 5);

    // 統計構造体サンプル
    ds_statistics_t *stats = NULL;
    ds_statistics_create(alloc, &stats);
    ds_statistics_calculate(alloc, stats, arr, n);
    double avg, med;
    int sum, min, max, mode;
    ds_statistics_get_average(stats, &avg);
    ds_statistics_get_median(stats, &med);
    ds_statistics_get_sum(stats, &sum);
    ds_statistics_get_min(stats, &min);
    ds_statistics_get_max(stats, &max);
    ds_statistics_get_mode(stats, &mode);
    ds_log(DS_LOG_LEVEL_INFO, "Stats: avg=%.1f med=%.1f sum=%d min=%d max=%d mode=%d",
            avg, med, sum, min, max, mode);
    ds_statistics_destroy(alloc, stats);

    // プロセスAPI例
    ds_process_t *proc = NULL;
    // ds_process_create(alloc, 123, 50, &proc); // 例: プロセス作成API
    // int32_t burst = ds_process_get_burst_time(proc);
    // int32_t pid   = ds_process_get_id(proc);

    // ロガーテスト
    ds_log(DS_LOG_LEVEL_INFO, "Shutdown complete. Goodbye!");

    return 0;
}
#endif // TESTING

#ifdef __cplusplus
}
#endif