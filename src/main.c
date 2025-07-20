<<<<<<< HEAD
<<<<<<< HEAD
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
=======
#include <stdio.h>
>>>>>>> feature
#include <stdbool.h>
#include <stdio.h>
#include "ds/statistics.h"
#include "data_structures.h" 
#include "util/logger.h" 
#include <assert.h>
>>>>>>> feature

<<<<<<< HEAD
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

<<<<<<< HEAD
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
=======
=======
=======
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

<<<<<<< HEAD
#include "data_structures.h"   /* DS_API_VERSION ほか共通型 */
#include "util/logger.h"       /* ds_log()                 */
#include "util/memory.h"       /* ds_get_default_allocator (実装側で用意されている前提) */
>>>>>>> feature

#include <stdio.h>   /* (void)argc 等の警告抑制にだけ使用。入出力はロガーへ */
=======
#include "data_structures.h"      // APIバージョン, Opaque型, 共通定義
#include "util/logger.h"          // ds_log()
#include "util/memory.h"          // ds_get_default_allocator()
>>>>>>> feature

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TESTING  // CIテスト時は除外
int main(int argc, char *argv[])
{
    (void)argc; (void)argv;  // 未使用警告抑止

    // ---- DI: アロケータ取得 ----
    const ds_allocator_t *alloc =
#ifdef DS_HAVE_DEFAULT_ALLOCATOR
        ds_get_default_allocator();
#else
        NULL; // Fallback（下層で標準malloc/free）
#endif
    (void)alloc; // 現状未使用。全APIの第1引数がallocであること！

<<<<<<< HEAD
    /* ---- 起動ログ ---- */
    ds_log(DS_LOG_LEVEL_INFO,
           "Data-Structures Library %s — application start",
           DS_API_VERSION);
>>>>>>> feature
=======
    // ---- 起動ログ ----
    ds_log(DS_LOG_LEVEL_INFO, "Data-Structures Library v%s — Startup", DS_API_VERSION);
>>>>>>> feature

    // ================= CLI/AI/VM/DB/Cloud/Undo-Redo起動点 ===================
    // 例:
    //    cli_dispatch(argc, argv, alloc);
    //    ai_model_bootstrap(alloc);
    //    db_migrate_if_needed();
    //    cloud_service_start(...);
    //    undo_redo_system_init();
    // ========================================================================

    ds_log(DS_LOG_LEVEL_INFO, "Shutdown complete. Goodbye!");
    return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature

<<<<<<< HEAD
<<<<<<< HEAD
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
=======
#endif /* TESTING */
>>>>>>> feature
=======
#endif /* TESTING */
>>>>>>> feature
=======
#endif /* TESTING */

#ifdef __cplusplus
} /* extern "C" */
#endif
>>>>>>> feature
=======
#endif // TESTING

#ifdef __cplusplus
}
#endif
>>>>>>> feature
