<<<<<<< HEAD
<<<<<<< HEAD:tests/test_url_shortener.c
#include "../include/ds/url_shortener.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
=======
#include "data_structures.h"
#include <assert.h>
#include <stdio.h>
=======
/**
 * @file    tests/ds/test_url_shortener.c
 * @brief   ds_url_shortener モジュールのユニットテスト
 * @note    main() は tests/test_main.c に集約される
 */

#include "ds/url_shortener.h"
#include "util/test_util.h"
#include "util/logger.h"
<<<<<<< HEAD
>>>>>>> feature
#include <string.h>
=======
>>>>>>> feature

#include <string.h>   /* strlen / strcmp */

/* ──────────────────────────────── */
/*  グローバル DI アロケータ        */
/*  tests/test_main.c で設定想定     */
extern const ds_allocator_t *g_alloc;

/* ──────────────────────────────── */
/*  共通アサート                    */
#define DS_TEST_ASSERT(cond, msg)                                              \
    do {                                                                       \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO,  "[PASS] %s", msg); }            \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)",            \
                           msg, __FILE__, __LINE__); }                         \
    } while (0)

/* ──────────────────────────────── */
/*  基本動作テスト                  */
void test__url_shortener_basic(void)
{
    ds_error_t          rc;
    ds_url_shortener_t *us = NULL;

<<<<<<< HEAD
<<<<<<< HEAD
    url_shortener_destroy(s);
    free(id);
    printf("test_url_shortener: OK\n");
}
>>>>>>> feature:tests/ds/test_url_shortener.c

int main(void) {
    ds_url_shortener_t* s = ds_url_shortener_create();

    char short_code[16] = {0};
    char expanded[256] = {0};

    // 登録・短縮
    ds_error_t r = ds_url_shortener_shorten(s, "https://google.com", short_code, sizeof(short_code));
    assert(r == DS_SUCCESS);
    assert(strlen(short_code) > 0);

    // 展開
    r = ds_url_shortener_expand(s, short_code, expanded, sizeof(expanded));
    assert(r == DS_SUCCESS);
    assert(strcmp(expanded, "https://google.com") == 0);

    ds_url_shortener_destroy(s);
    printf("test_url_shortener: OK\n");
    return 0;
}

=======
    // --- インスタンス生成 ---
    err = ds_url_shortener_create(10, &us);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(us != NULL, "create: not NULL");
=======
    /* --- 生成（@ownership: caller frees） --- */
    rc = ds_url_shortener_create(g_alloc, 10, &us);
    DS_TEST_ASSERT(rc == DS_SUCCESS && us, "create");
>>>>>>> feature

    /* --- shorten / expand 正常系 --- */
    const char *orig = "https://www.openai.com/product/gpt";
    char short_url[128] = {0};
    char expanded [256] = {0};

    rc = ds_url_shortener_shorten(us, orig, short_url, sizeof short_url);
    DS_TEST_ASSERT(rc == DS_SUCCESS && strlen(short_url) > 0, "shorten");

    rc = ds_url_shortener_expand(us, short_url, expanded, sizeof expanded);
    DS_TEST_ASSERT(rc == DS_SUCCESS && strcmp(orig, expanded) == 0, "expand");

    /* --- 破棄 --- */
    rc = ds_url_shortener_destroy(us);
    DS_TEST_ASSERT(rc == DS_SUCCESS, "destroy");
}

/* ──────────────────────────────── */
/*  異常/境界条件テスト             */
void test__url_shortener_edge_cases(void)
{
    ds_error_t          rc;
    ds_url_shortener_t *us = NULL;
    char out1[16] = {0}, out2[32] = {0};

    /* NULL pointer 系 */
    rc = ds_url_shortener_shorten(NULL, "http://x", out1, sizeof out1);
    DS_TEST_ASSERT(rc == DS_ERR_NULL_POINTER, "shorten: NULL handle");

<<<<<<< HEAD
    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_url_shortener_basic 完了");
}
>>>>>>> feature
=======
    rc = ds_url_shortener_shorten(us, NULL, out1, sizeof out1);
    DS_TEST_ASSERT(rc == DS_ERR_NULL_POINTER, "shorten: NULL url");

    rc = ds_url_shortener_expand(NULL, "abc", out2, sizeof out2);
    DS_TEST_ASSERT(rc == DS_ERR_NULL_POINTER, "expand: NULL handle");

    rc = ds_url_shortener_expand(us, NULL, out2, sizeof out2);
    DS_TEST_ASSERT(rc == DS_ERR_NULL_POINTER, "expand: NULL key");

    /* バッファ不足 */
    ds_url_shortener_create(g_alloc, 4, &us);
    rc = ds_url_shortener_shorten(us, "http://example.com", out1, 2); /* too small */
    DS_TEST_ASSERT(rc == DS_ERR_OUT_OF_RANGE, "shorten: small buf");

    rc = ds_url_shortener_expand(us, "aaaa", out2, 3); /* too small */
    DS_TEST_ASSERT(rc == DS_ERR_OUT_OF_RANGE, "expand: small buf");

    ds_url_shortener_destroy(us);
}
>>>>>>> feature
