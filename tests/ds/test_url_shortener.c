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

int main(void) {
    test_url_shortener();
    return 0;
}