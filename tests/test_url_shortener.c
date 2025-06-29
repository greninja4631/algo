#include "../include/ds/url_shortener.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

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

