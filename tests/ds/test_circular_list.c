/**
 * @file    tests/ds/test_circular_list.c
 * @brief   ds_circular_list モジュール単体テスト
 * @details 本ファイルは test_main.c から呼ばれる。実装/ガイドライン：2025-07 完全準拠。
 */

#include <stdlib.h>                 /* calloc / free */
#include "ds/circular_list.h"       /* 被テストAPI */
#include "util/logger.h"            /* ds_log() */

/*─────────────────────────────────────*
 * [1] テスト用アロケータDI（安全/再利用型）
 *─────────────────────────────────────*/
/**
 * @brief calloc(NULL safe) 経由の汎用アロケータ
 * @details cnt==0時はNULL返却、free(NULL)も安全
 */
static void* _std_alloc(size_t cnt, size_t sz) { return cnt ? calloc(cnt, sz) : NULL; }
static void  _std_free (void*  p)              { if (p) free(p); }

static const ds_allocator_t g_alloc_impl = {
    .alloc = _std_alloc,
    .free  = _std_free
};
#define G_ALLOC (&g_alloc_impl)

/*─────────────────────────────────────*
 * [2] アサーション・共通テストマクロ
 *─────────────────────────────────────*/
#define TASSERT(cond, msg)                                                      \
    do {                                                                        \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO , "[PASS] %s", (msg)); }           \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",             \
                           (msg), __FILE__, __LINE__); }                        \
    } while (0)

/*─────────────────────────────────────*
 * [3] 基本動作テスト（全APIカバレッジ、CI用）
 *─────────────────────────────────────*/
void test__circular_list_basic(void)
{
    ds_circular_list_t *out_list = NULL;
    void               *out_data = NULL;
    int                  a = 10, b = 20, c = 30;

    // --- Create (ownership: caller frees via destroy)
    TASSERT(ds_circular_list_create(G_ALLOC, &out_list) == DS_SUCCESS, "create");
    TASSERT(out_list != NULL,                               "list != NULL after create");
    TASSERT(ds_circular_list_is_empty(out_list),            "empty after create");
    TASSERT(ds_circular_list_size(out_list) == 0,           "size==0 after create");

    // --- Insert (ownership: transfer)
    TASSERT(ds_circular_list_insert(G_ALLOC, out_list, &a) == DS_SUCCESS, "insert a");
    TASSERT(ds_circular_list_insert(G_ALLOC, out_list, &b) == DS_SUCCESS, "insert b");
    TASSERT(ds_circular_list_insert(G_ALLOC, out_list, &c) == DS_SUCCESS, "insert c");
    TASSERT(ds_circular_list_size(out_list) == 3,                         "size==3 after 3 insert");

    // --- Remove (ownership: transfer back, FIFO/rotation)
    TASSERT(ds_circular_list_remove(G_ALLOC, out_list, &out_data) == DS_SUCCESS && *(int*)out_data == a, "remove a");
    TASSERT(ds_circular_list_remove(G_ALLOC, out_list, &out_data) == DS_SUCCESS && *(int*)out_data == b, "remove b");
    TASSERT(ds_circular_list_remove(G_ALLOC, out_list, &out_data) == DS_SUCCESS && *(int*)out_data == c, "remove c");
    TASSERT(ds_circular_list_is_empty(out_list),                           "empty after all remove");

    // --- Remove on empty (ownership: no effect)
    TASSERT(ds_circular_list_remove(G_ALLOC, out_list, &out_data) == DS_ERR_EMPTY, "remove from empty");

    // --- Destroy (ownership: callee frees)
    TASSERT(ds_circular_list_destroy(G_ALLOC, out_list) == DS_SUCCESS, "destroy after empty");
    // "out_list"はローカル変数でポインタ値のまま（安全上、外部セット不要）

    // --- NULL-safety checks（全APIで対応必須）
    TASSERT(ds_circular_list_is_empty(NULL),                 "is_empty(NULL) safe");
    TASSERT(ds_circular_list_size(NULL) == 0,                "size(NULL) == 0");
    TASSERT(ds_circular_list_destroy(G_ALLOC, NULL) == DS_ERR_NULL_POINTER, "destroy(NULL)");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] test__circular_list_basic finished");
}

/*─────────────────────────────────────*
 * [4] （将来拡張）リソース解放/Valgrind対応
 *─────────────────────────────────────*/
/*
 * test__circular_list_teardown(void)
 * → 全リソース回収/リークゼロ保証（Valgrindで必須）
 * → teardown時に全ノード/本体freeされること確認
 * → 手動解放以外はCIビルドでfailさせる設計
 */

/*─────────────────────────────────────*
 * [5] テスト拡張例（Edge/Stress/Stats Coverage）
 *─────────────────────────────────────*/
// void test__circular_list_edge_cases(void) { ... }
// void test__circular_list_stats(void)      { ... }
// CI本体/runner側で必ず全テスト実行されるようにtest_main.cで集約すること。

/*─────────────────────────────────────*
 * [6] テスト用APIエクスポート
 *─────────────────────────────────────*/
#ifdef EXPORT_CIRCULAR_LIST_TESTS
int main(void) {
    test__circular_list_basic();
    // test__circular_list_edge_cases();
    // test__circular_list_stats();
    return 0;
}
#endif