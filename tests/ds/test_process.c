/**
 * @file    tests/ds/process_test.c
 * @brief   プロセス管理APIの単体テスト（test_main.cから呼ばれる）
 *
 * - ds_process_create/destroy/getterの基本・異常系
 * - アロケータは標準calloc/freeを抽象DIでラップ
 * - ロギングはds_logのみ
 * - テストmain分離（main持たない。test_main.cで宣言呼び出し）
 */

#include <stddef.h>
#include <stdlib.h>
#include "ds/process.h"
#include "util/logger.h"

/* DIアロケータ（標準calloc/freeラップ） */
static void* _std_alloc(size_t n, size_t sz) { return calloc(n, sz); }
static void  _std_free (void* p)             { free(p); }
static const ds_allocator_t g_alloc = { .alloc = _std_alloc, .free = _std_free };
#define G_ALLOC (&g_alloc)

/* 共通アサーション（色つき） */
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); \
        else      ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); \
    } while (0)

/* 基本動作テスト */
void ds_test_process_basic(void)
{
    ds_process_t *proc = NULL;
    DS_TEST_ASSERT(ds_process_create(G_ALLOC, 123, 45, &proc) == DS_SUCCESS, "create");
    DS_TEST_ASSERT(proc,                                       "proc != NULL");
    DS_TEST_ASSERT(ds_process_get_id(proc) == 123,             "id getter");
    DS_TEST_ASSERT(ds_process_get_burst_time(proc) == 45,      "burst getter");
    DS_TEST_ASSERT(ds_process_destroy(G_ALLOC, proc) == DS_SUCCESS, "destroy");
}

/* 異常系テスト */
void ds_test_process_edge_cases(void)
{
    /* NULL out ptr */
    DS_TEST_ASSERT(ds_process_create(G_ALLOC, 1, 2, NULL) == DS_ERR_NULL_POINTER, "create: NULL out");
    /* NULL destroyはno-opで成功 */
    DS_TEST_ASSERT(ds_process_destroy(G_ALLOC, NULL) == DS_SUCCESS, "destroy: NULL");
}

/* test_main.cのプロトタイプ宣言に
   void ds_test_process_basic(void);
   void ds_test_process_edge_cases(void);
   を追加することで、未使用関数もゼロ保証。
 */