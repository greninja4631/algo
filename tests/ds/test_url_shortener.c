/**
 * @file test_url_shortener.c
 * @brief ds_url_shortener_t の単体テスト
 * @details 本ファイルは test_main.c から呼び出される（main関数は持たない）
 */

#include "ds/url_shortener.h"
#include "util/test_util.h"
#include "util/logger.h"
#include <string.h>

// テスト用アサーション
#define DS_TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { ds_log(DS_LOG_LEVEL_INFO, "[PASS] %s", msg); } \
        else      { ds_log(DS_LOG_LEVEL_ERROR, "[FAIL] %s (%s:%d)", msg, __FILE__, __LINE__); } \
    } while (0)

void ds_test_url_shortener_basic(void)
{
    ds_error_t err;
    ds_url_shortener_t* us = NULL;

    // --- インスタンス生成 ---
    err = ds_url_shortener_create(10, &us);
    DS_TEST_ASSERT(err == DS_SUCCESS, "create: DS_SUCCESS");
    DS_TEST_ASSERT(us != NULL, "create: not NULL");

    // --- 短縮 & 展開テスト ---
    const char* original = "https://www.openai.com/product/gpt";
    char short_url[128] = {0};
    char expanded[256] = {0};

    // shorten
    err = ds_url_shortener_shorten(us, original, short_url, sizeof(short_url));
    DS_TEST_ASSERT(err == DS_SUCCESS, "shorten: DS_SUCCESS");
    DS_TEST_ASSERT(strlen(short_url) > 0, "shorten: non-empty");

    // expand
    err = ds_url_shortener_expand(us, short_url, expanded, sizeof(expanded));
    DS_TEST_ASSERT(err == DS_SUCCESS, "expand: DS_SUCCESS");
    DS_TEST_ASSERT(strcmp(original, expanded) == 0, "expand: matches original");

    // --- 空/NULL入力 ---
    err = ds_url_shortener_shorten(NULL, original, short_url, sizeof(short_url));
    DS_TEST_ASSERT(err != DS_SUCCESS, "shorten: NULL us");
    err = ds_url_shortener_shorten(us, NULL, short_url, sizeof(short_url));
    DS_TEST_ASSERT(err != DS_SUCCESS, "shorten: NULL url");
    err = ds_url_shortener_shorten(us, original, NULL, sizeof(short_url));
    DS_TEST_ASSERT(err != DS_SUCCESS, "shorten: NULL out_short");

    err = ds_url_shortener_expand(NULL, short_url, expanded, sizeof(expanded));
    DS_TEST_ASSERT(err != DS_SUCCESS, "expand: NULL us");
    err = ds_url_shortener_expand(us, NULL, expanded, sizeof(expanded));
    DS_TEST_ASSERT(err != DS_SUCCESS, "expand: NULL short_url");
    err = ds_url_shortener_expand(us, short_url, NULL, sizeof(expanded));
    DS_TEST_ASSERT(err != DS_SUCCESS, "expand: NULL out_url");

    // --- 破棄 ---
    err = ds_url_shortener_destroy(us);
    DS_TEST_ASSERT(err == DS_SUCCESS, "destroy: DS_SUCCESS");
    DS_TEST_ASSERT(ds_url_shortener_destroy(NULL) != DS_SUCCESS, "destroy: NULL");

    ds_log(DS_LOG_LEVEL_INFO, "[OK] ds_test_url_shortener_basic 完了");
}