#include "lru_cache.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_lru_basic() {
    lru_cache_t* c = lru_cache_create(2);
    assert(c);

    lru_cache_put(c, "k1", strdup("v1"));
    lru_cache_put(c, "k2", strdup("v2"));

    void* v = NULL;
    assert(lru_cache_get(c, "k1", &v) == HM_OK);
    assert(strcmp((char*)v, "v1") == 0);

    lru_cache_put(c, "k3", strdup("v3")); // "k2" 追い出し

    assert(lru_cache_get(c, "k2", &v) != HM_OK); // "k2"はキャッシュから消えている
    lru_cache_destroy(c);
    printf("test_lru_basic: OK\n");
}

int main(void) {
    test_lru_basic();
    return 0;
}