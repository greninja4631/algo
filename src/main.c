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
 * @brief   ライブラリ実行バイナリのエントリーポイント
 *
 * ──────────────────────────────────────────────
 *  ビルド時フラグ
 *  --------------------------------------------
 *  • -DTESTING  を付けると  main() を除外（単体テスト用）
 *
 *  ガイドライン適合ポイント
 *  --------------------------------------------
 *  ✔ 標準 C シグネチャ      : int main(int argc, char *argv[])
 *  ✔ 直接 printf 禁止       : ds_log() でログ出力
 *  ✔ 依存性逆転 (DI)        : アロケータは ds_get_default_allocator() で取得想定
 *  ✔ グローバル状態なし     : 必要なら静的ローカルで管理
 *  ✔ 戻り値 0 = 正常終了
 * ──────────────────────────────────────────────
 */

#include "data_structures.h"   /* DS_API_VERSION ほか共通型 */
#include "util/logger.h"       /* ds_log()                 */
#include "util/memory.h"       /* ds_get_default_allocator (実装側で用意されている前提) */
>>>>>>> feature

#include <stdio.h>   /* (void)argc 等の警告抑制にだけ使用。入出力はロガーへ */

#ifdef __cplusplus
extern "C" {
#endif

/*======================================================================*/
/*  production main                                                     */
/*======================================================================*/
#ifndef TESTING        /* 単体テストビルド時は main を含めない  */
int main(int argc, char *argv[])
{
    /* ---- 未使用警告抑制（将来 CLI 解析を追加する場合は削除） ---- */
    (void)argc;
    (void)argv;

    /* ---- 依存性逆転：デフォルトアロケータを取得 ----
       プロジェクトで ds_get_default_allocator() が未実装なら
       NULL を使い、下層で標準 malloc/free が利用される */
#ifdef DS_HAVE_DEFAULT_ALLOCATOR
    const ds_allocator_t *alloc = ds_get_default_allocator();
#else
    const ds_allocator_t *alloc = NULL;
#endif
    (void)alloc;  /* いまは未使用 */

    /* ---- 起動ログ ---- */
    ds_log(DS_LOG_LEVEL_INFO,
           "Data-Structures Library %s — application start",
           DS_API_VERSION);
>>>>>>> feature

    /* ==============================================================
     * TODO: ここにアプリ本体の初期化 / CLI 解析 / サブコマンド呼び出し
     *       などを追加してください。
     * ============================================================== */

    /* ---- 正常終了ログ ---- */
    ds_log(DS_LOG_LEVEL_INFO, "Shutdown complete. Goodbye!");
    return 0;
}
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
