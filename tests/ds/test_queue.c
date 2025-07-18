/*
 * tests/ds/test_queue.c
 * ────────────────────────────────────────────────────────────
 *  ds_queue モジュール単体テスト
 *  2025-07 ガイドライン準拠
 *
 *  @ownership caller frees
 *────────────────────────────────────────────────────────────*/
#include <stdlib.h>
#include "ds/queue.h"     /* 被テスト API */
#include "util/logger.h"  /* ds_log */

/*───────────────────────────────────────────────
 * 1. アロケータ DI（calloc/free は NULL 安全）
 *───────────────────────────────────────────────*/
static void* _std_alloc(size_t cnt, size_t sz) {            /* cnt==0 → NULL */
    return cnt ? calloc(cnt, sz) : NULL;
}
static void  _std_free (void* p) {                          /* free(NULL) OK  */
    if (p) free(p);
}
static const ds_allocator_t g_alloc_impl = {
    .alloc = _std_alloc,
    .free  = _std_free
};
#define G_ALLOC (&g_alloc_impl)

/*───────────────────────────────────────────────
 * 2. 共通アサーション（TRACE/DEBUG/INFO/…6段準拠）
 *───────────────────────────────────────────────*/
#define TASSERT(cond, msg)                                                   \
    do {                                                                     \
        if (cond) ds_log(DS_LOG_LEVEL_INFO , "[PASS] %s", (msg));            \
        else      ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",            \
                         (msg), __FILE__, __LINE__);                         \
    } while (0)

/*───────────────────────────────────────────────
 * 3-A. 基本機能テスト
 *───────────────────────────────────────────────*/
void test__queue_basic(void)
{
    ds_queue_t *q = NULL;
    void       *data = NULL;
    int a = 11, b = 22, c = 33;

    /* create */
    TASSERT(ds_queue_create(G_ALLOC, &q) == DS_SUCCESS && q, "create");

    /* enqueue */
    TASSERT(ds_queue_enqueue(G_ALLOC, q, &a) == DS_SUCCESS, "enq a");
    TASSERT(ds_queue_enqueue(G_ALLOC, q, &b) == DS_SUCCESS, "enq b");
    TASSERT(ds_queue_enqueue(G_ALLOC, q, &c) == DS_SUCCESS, "enq c");
    TASSERT(ds_queue_size(q) == 3, "size 3");

    /* front / dequeue in FIFO order */
    TASSERT(ds_queue_front(q, &data) == DS_SUCCESS && *(int*)data == a, "front a");
    TASSERT(ds_queue_dequeue(G_ALLOC, q, &data) == DS_SUCCESS && *(int*)data == a, "deq a");
    TASSERT(ds_queue_dequeue(G_ALLOC, q, &data) == DS_SUCCESS && *(int*)data == b, "deq b");
    TASSERT(ds_queue_dequeue(G_ALLOC, q, &data) == DS_SUCCESS && *(int*)data == c, "deq c");
    TASSERT(ds_queue_is_empty(q), "empty");

    /* empty dequeue */
    TASSERT(ds_queue_dequeue(G_ALLOC, q, &data) == DS_ERR_EMPTY, "deq empty");

    /* destroy */
    TASSERT(ds_queue_destroy(G_ALLOC, q)   == DS_SUCCESS,        "destroy");
    TASSERT(ds_queue_destroy(G_ALLOC, NULL) == DS_ERR_NULL_POINTER, "destroy NULL");
}

/*───────────────────────────────────────────────
 * 3-B. NULL／異常系テスト
 *───────────────────────────────────────────────*/
void test__queue_edge_cases(void)
{
    void *dummy = NULL;

    TASSERT(ds_queue_is_empty(NULL),                   "is_empty NULL");
    TASSERT(ds_queue_size(NULL) == 0,                  "size NULL");
    TASSERT(ds_queue_front(NULL, &dummy) == DS_ERR_NULL_POINTER, "front NULL");
    TASSERT(ds_queue_dequeue(G_ALLOC, NULL, &dummy) == DS_ERR_NULL_POINTER, "deq NULL");
    TASSERT(ds_queue_enqueue(G_ALLOC, NULL, &dummy) == DS_ERR_NULL_POINTER, "enq NULL");
}

/*───────────────────────────────────────────────
 * 4. スタンドアロン実行用 main
 *    （CI では tests/test_main.c が一括呼び出し）
 *───────────────────────────────────────────────*/
#ifdef DS_STANDALONE_TEST
int main(void)
{
    test__queue_basic();
    test__queue_edge_cases();
    return 0;
}
#endif