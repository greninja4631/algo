<<<<<<< HEAD:tests/test_url_shortener.c
#include "../include/ds/url_shortener.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
=======
#include "data_structures.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "ds/url_shortener.h"  // シンプルなサンプルのため直接include

void test_url_shortener() {
    url_shortener_t* s = url_shortener_create(10);

    char* id = url_shortener_add(s, "https://google.com");
    assert(id);
    const char* url = url_shortener_get(s, id);
    assert(url && strcmp(url, "https://google.com") == 0);

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

