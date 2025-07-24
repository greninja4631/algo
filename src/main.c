/**
 * @file    src/main.c
 * @brief   ライブラリ本体の「唯一の」エントリポイント（プロダクション用）
 */

#include "data_structures.h"
#include "util/logger.h"
#include "util/memory.h"
#include "algo/search.h"
#include "algo/sort.h"
#include "ds/statistics.h"
// #include "ds/process.h"  // 未使用ならインクルード不要

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    // アロケータ（全API第1引数に必要なものだけ渡す！）
    const ds_allocator_t *alloc =
#ifdef DS_HAVE_DEFAULT_ALLOCATOR
        ds_get_default_allocator();
#else
        NULL;
#endif

    ds_log(DS_LOG_LEVEL_INFO, "Data-Structures Library v%s — Startup", DS_API_VERSION);

    // --- サンプルAPI利用例 ---
    int arr[] = {10, 20, 30, 40, 50};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    ptrdiff_t idx = -1;
    ds_error_t err;

    // 線形探索
    err = ds_search_linear(arr, n, 30, &idx);
    ds_log(DS_LOG_LEVEL_INFO, "Search 30: err=%d, idx=%td", err, idx);

    // 二分探索
    err = ds_search_binary(arr, n, 40, &idx);
    ds_log(DS_LOG_LEVEL_INFO, "Binary search 40: err=%d, idx=%td", err, idx);

    // ソート
    int unsorted[] = {9, 1, 7, 3, 5};
    ds_sort_bubble(unsorted, 5);
    ds_sort_insertion(unsorted, 5);

    // 統計構造体サンプル
    ds_statistics_t *stats = NULL;
    ds_statistics_create(alloc, &stats);
    ds_statistics_calculate(stats, arr, n);
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

    // 未使用関数は絶対呼ばない！（コメントで「使う時のみ有効化」と明示）
    // ds_process_t *proc = NULL;
    // ds_process_create(alloc, 123, 50, &proc);

    ds_log(DS_LOG_LEVEL_INFO, "Shutdown complete. Goodbye!");
    return 0;
}
