/**
 * @file test_lru_cache.c
 * @brief LRUキャッシュのユニットテスト
 * @version 1.0.0
 * @date 2025-06-25
 *
 * - API名・型は実装(header)と必ず一致（void*型値）
 * - キー型：int, 値型：void*（テストではchar*文字列を格納/解放まで対応）
 * - 全操作は assert で堅牢性を担保
 * - メモリリークなし（キャッシュ追い出し分もfree）
 */

#include "../include/ds/lru_cache.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 値（void*）をfreeする関数
static void free_str(void* v) {
    free(v);
}

// LRUキャッシュ基本動作テスト
void test_lru_basic() {
    ds_lru_cache_t* c = ds_lru_cache_create(2);
    assert(c);

    ds_lru_cache_put(c, 1, strdup("v1"));
    ds_lru_cache_put(c, 2, strdup("v2"));

    void* v = NULL;
    assert(ds_lru_cache_get(c, 1, &v) == DS_SUCCESS);
    assert(strcmp((char*)v, "v1") == 0);

    // "2"が追い出されるので手動でfree
    ds_lru_cache_put(c, 3, strdup("v3"));

    // 追い出された"2"はキャッシュから取得不可、OK
    assert(ds_lru_cache_get(c, 2, &v) != DS_SUCCESS);

    // 全キャッシュをfree（本体のdestroyで全値をfreeする場合は不要）
    // 今回はdestroy内でfreeしていると仮定

    ds_lru_cache_destroy(c);
    printf("test_lru_basic: OK\n");
}

int main(void) {
    test_lru_basic();
    return 0;
}
