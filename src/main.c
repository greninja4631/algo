<<<<<<< HEAD
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../include/data_structures.h"
#include "../include/ds/statistics.h"



// 各種テスト関数のプロトタイプ宣言（実体は他のファイルにて実装済みでOK）
ds_error_t demonstrate_array_operations(void);
ds_error_t test_stack_operations(void);
ds_error_t test_queue_operations(void);
ds_error_t test_undo_redo_system(void);
ds_error_t test_circular_and_round_robin(void);
=======
#include <stdbool.h>
#include <stdio.h>
#include "statistics.h"
#include <assert.h>
>>>>>>> feature

int main(void) {
    printf("=================================================\n");
    printf("Production-Ready Data Structures System\n");
    printf("Version: 1.0.0\nBuild: %s %s\n", __DATE__, __TIME__);
    printf("=================================================\n");

    ds_error_t overall_result = DS_SUCCESS;

    // ★★ 統計APIの動作例 ★★
    printf("\n=== [Statistics Demo] ===\n");
    int data[] = {1, 2, 3, 3, 5, 5, 5, 8, 9};
    size_t size = sizeof(data) / sizeof(data[0]);
    ds_statistics_t stats = ds_calculate_statistics(data, size);

    // JSON形式で出力
    printf("{\n");
    printf("  \"sum\": %d,\n", stats.sum);
    printf("  \"min\": %d,\n", stats.min);
    printf("  \"max\": %d,\n", stats.max);
    printf("  \"average\": %.2f,\n", stats.average);
    printf("  \"median\": %.2f,\n", stats.median);
    printf("  \"mode\": %d\n", stats.mode);
    printf("}\n");

    // === データ構造のテストスイート ===
    struct {
        const char *test_name;
        ds_error_t (*test_func)(void);
    } test_suite[] = {
        {"Array Operations Demo", demonstrate_array_operations},
        {"Stack Operations", test_stack_operations},
        {"Queue Operations", test_queue_operations},
        {"Undo/Redo System", test_undo_redo_system},
        {"Circular List & Round Robin", test_circular_and_round_robin}
    };

    size_t num_tests = sizeof(test_suite) / sizeof(test_suite[0]);
    size_t passed_tests = 0;

    for (size_t i = 0; i < num_tests; i++) {
        printf("\n[TEST %zu/%zu] %s\n----------------------------------------\n", i + 1, num_tests, test_suite[i].test_name);
        ds_error_t r = test_suite[i].test_func();
        if (r == DS_SUCCESS) {
            printf("✅ PASSED: %s\n", test_suite[i].test_name);
            passed_tests++;
        } else {
            printf("❌ FAILED: %s (Error code: %d)\n", test_suite[i].test_name, r);
            overall_result = r;
        }
    }

    printf("\n=================================================\n");
    printf("TEST SUMMARY\n");
    printf("=================================================\n");
    printf("Total tests: %zu\n", num_tests);
    printf("Passed: %zu\n", passed_tests);
    printf("Failed: %zu\n", num_tests - passed_tests);

    if (overall_result == DS_SUCCESS) {
        printf("🎉 ALL TESTS PASSED\n");
    } else {
        printf("💥 SOME TESTS FAILED\n");
    }

    // 本番環境での監視情報出力
    printf("\n=== Production Monitoring Info ===\n");
    printf("Timestamp: %ld\n", (long)time(NULL));
    printf("Process ID: %d\n", getpid());
    printf("Exit code: %d\n", overall_result == DS_SUCCESS ? 0 : 1);

    printf("\n=== Application Terminated ===\n");
    return overall_result == DS_SUCCESS ? 0 : 1;
}