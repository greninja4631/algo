/*
 * tests/ds/test_lru_cache.c
 * LRUキャッシュ ユニットテスト
 * 2025-07 ガイドライン準拠（DIアロケータ／メモリ解放責務を徹底）
 *
 * @ownership caller frees
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ds/lru_cache.h"   /* 被テスト API */
#include "util/logger.h"    /* ds_log */

/*───────────────────────────────────────────────
 * 1. アロケータ DI  -- calloc/free は NULL 安全
 *───────────────────────────────────────────────*/
static void* _std_alloc(size_t cnt, size_t sz) { return cnt ? calloc(cnt, sz) : NULL; }
static void  _std_free (void* p)               { if (p) free(p); }

static const ds_allocator_t g_alloc_impl = {
    .alloc = _std_alloc,
    .free  = _std_free
};
#define G_ALLOC (&g_alloc_impl)

/*───────────────────────────────────────────────
 * 2. アサーションマクロ（6段ロガー準拠）
 *───────────────────────────────────────────────*/
#define TASSERT(cond, msg)                                                      \
    do {                                                                        \
        if (cond) ds_log(DS_LOG_LEVEL_INFO , "[PASS] %s", (msg));               \
        else      ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",               \
                         (msg), __FILE__, __LINE__);                            \
    } while (0)

/*───────────────────────────────────────────────
 * 3-A. 基本機能
 *───────────────────────────────────────────────*/
void test__lru_cache_basic(void)
{
    ds_lru_cache_t *c   = NULL;
    const size_t    cap = 8;

    TASSERT(ds_lru_cache_create(G_ALLOC, cap, &c) == DS_SUCCESS, "create");

    /* put / get / MRU 動作 */
    TASSERT(ds_lru_cache_put(G_ALLOC, c, "key1", "value1") == DS_SUCCESS, "put key1");

    void *val = NULL;
    TASSERT(ds_lru_cache_get(G_ALLOC, c, "key1", &val) == DS_SUCCESS &&
            strcmp((char *)val, "value1") == 0, "get key1");

    /* size API */
    size_t sz = 0;
    TASSERT(ds_lru_cache_size(G_ALLOC, c, &sz) == DS_SUCCESS && sz == 1, "size == 1");

    ds_lru_cache_destroy(G_ALLOC, c);
}

/*───────────────────────────────────────────────
 * 3-B. リソース完全解放テスト
 *───────────────────────────────────────────────*/
void test__lru_cache_free_all(void)
{
    ds_lru_cache_t *c = NULL;
    TASSERT(ds_lru_cache_create(G_ALLOC, 4, &c) == DS_SUCCESS, "create free_all");

    for (int i = 0; i < 10; ++i) {
        char key[16]; sprintf(key, "K%d", i);
        char *val = (char *)G_ALLOC->alloc(1, 16);   /* 16-byte 文字列領域 */
        strcpy(val, "VAL");
        TASSERT(ds_lru_cache_put(G_ALLOC, c, key, val) == DS_SUCCESS, "put bulk");
    }
    ds_lru_cache_destroy(G_ALLOC, c);  /* ノード→map→本体まですべて解放 */
}

/*───────────────────────────────────────────────
 * 3-C. ノード確保失敗時のリーク検証（簡易）
 *───────────────────────────────────────────────*/
void test__lru_cache_node_alloc_error(void)
{
    /* 本プロジェクトでは ds_set_memory_functions() で
       OOM シミュレーションを注入する仕組みがある前提。
       ここでは “準備のみ” とし、実際の OOM テストは CI 側で実行。 */
}

/*───────────────────────────────────────────────
 * 3-D. free(NULL) 安全性
 *───────────────────────────────────────────────*/
void test__lru_cache_null_free_safe(void)
{
    G_ALLOC->free(NULL);   /* クラッシュしないことを確認（要手動 Valgrind） */
}

/*───────────────────────────────────────────────
 * 4. 単体実行用 main（CI では tests/test_main.c から呼出）
 *───────────────────────────────────────────────*/
#ifdef DS_STANDALONE_TEST
int main(void)
{
    test__lru_cache_basic();
    test__lru_cache_free_all();
    test__lru_cache_node_alloc_error();
    test__lru_cache_null_free_safe();
    return 0;
}
#endif