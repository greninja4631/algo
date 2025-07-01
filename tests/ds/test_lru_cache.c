#include "data_structures.h"
#include "ds/lru_cache.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>





void test_lru_basic() {
    ds_lru_cache_t* c = NULL;
    assert(ds_lru_cache_create(2, &c) == DS_SUCCESS);
    assert(c);

    assert(ds_lru_cache_put(c, "k1", strdup("v1")) == DS_SUCCESS);
    assert(ds_lru_cache_put(c, "k2", strdup("v2")) == DS_SUCCESS);

    void* v = NULL;
    assert(ds_lru_cache_get(c, "k1", &v) == DS_SUCCESS);
    assert(strcmp((char*)v, "v1") == 0);

    assert(ds_lru_cache_put(c, "k3", strdup("v3")) == DS_SUCCESS); // "k2"追い出し

    assert(ds_lru_cache_get(c, "k2", &v) == DS_ERR_NOT_FOUND); // "k2"は消えている
    assert(ds_lru_cache_destroy(c) == DS_SUCCESS);
    printf("test_lru_basic: OK\n");
}

int main(void) {
    test_lru_basic();
    return 0;
}