/**
 * @file test_queue.c
 * @brief ds_queue_t モジュールの単体テスト
 * @details 本ファイルは test_main.c から呼び出す。main関数は持たない。
 */

#include "ds/queue.h"
#include "util/test_util.h"
#include "util/logger.h"

// プロジェクト共通アサーションマクロ
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

/**
 * @brief 基本動作テスト
 */
void ds_test_queue_basic(void)
{
    ds_queue_t* queue = NULL;
    ds_error_t err;
    void* data = NULL;
    int a = 11, b = 22, c = 33;

    // --- 生成 ---
    err = ds_queue_create(&queue);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(queue != NULL, "create: not NULL");
    DS_TEST_ASSERT(ds_queue_is_empty(queue), "is_empty: after create");
    DS_TEST_ASSERT(ds_queue_size(queue) == 0, "size: after create");

    // --- 追加 ---
    err = ds_queue_enqueue(queue, &a);
    DS_TEST_ASSERT(err == DS_SUCCESS, "enqueue a");
    DS_TEST_ASSERT(!ds_queue_is_empty(queue), "not empty after enqueue");
    DS_TEST_ASSERT(ds_queue_size(queue) == 1, "size 1");

    err = ds_queue_enqueue(queue, &b);
    DS_TEST_ASSERT(err == DS_SUCCESS, "enqueue b");
    DS_TEST_ASSERT(ds_queue_size(queue) == 2, "size 2");

    err = ds_queue_enqueue(queue, &c);
    DS_TEST_ASSERT(err == DS_SUCCESS, "enqueue c");
    DS_TEST_ASSERT(ds_queue_size(queue) == 3, "size 3");

    // --- front ---
    err = ds_queue_front(queue, &data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)data == a, "front: a");

    // --- 取り出し ---
    err = ds_queue_dequeue(queue, &data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)data == a, "dequeue: a");
    DS_TEST_ASSERT(ds_queue_size(queue) == 2, "size 2 after dequeue a");

    err = ds_queue_dequeue(queue, &data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)data == b, "dequeue: b");
    DS_TEST_ASSERT(ds_queue_size(queue) == 1, "size 1 after dequeue b");

    err = ds_queue_dequeue(queue, &data);
    DS_TEST_ASSERT(err == DS_SUCCESS && *(int*)data == c, "dequeue: c");
    DS_TEST_ASSERT(ds_queue_size(queue) == 0, "size 0 after dequeue all");
    DS_TEST_ASSERT(ds_queue_is_empty(queue), "is_empty after all dequeue");

    // --- 空でdequeue ---
    err = ds_queue_dequeue(queue, &data);
    DS_TEST_ASSERT(err == DS_ERR_EMPTY, "dequeue: empty");

    // --- 破棄 ---
    err = ds_queue_destroy(queue);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
    DS_TEST_ASSERT(ds_queue_destroy(NULL) == DS_ERR_NULL_POINTER, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_queue_basic 完了");
}

/**
 * @brief NULL安全・異常系テスト
 */
void ds_test_queue_edge_cases(void)
{
    ds_error_t err;
    void* data = NULL;
    DS_TEST_ASSERT(ds_queue_is_empty(NULL), "is_empty: NULL");
    DS_TEST_ASSERT(ds_queue_size(NULL) == 0, "size: NULL");

    err = ds_queue_front(NULL, &data);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "front: NULL");

    err = ds_queue_dequeue(NULL, &data);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "dequeue: NULL");

    err = ds_queue_enqueue(NULL, NULL);
    DS_TEST_ASSERT(err == DS_ERR_NULL_POINTER, "enqueue: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_queue_edge_cases 完了");
}