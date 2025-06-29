#include "../include/ds/hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_value(void* v) { free(v); }

void test_hashmap_basic() {
    ds_hashmap_t* map = ds_hashmap_create(8, free, free_value);
    assert(map);

    assert(ds_hashmap_put(map, "hello", strdup("world")) == DS_SUCCESS);

    void* val = NULL;
    assert(ds_hashmap_get(map, "hello", &val) == DS_SUCCESS);
    assert(strcmp((char*)val, "world") == 0);

    assert(ds_hashmap_remove(map, "hello") == DS_SUCCESS);
    ds_hashmap_destroy(map);
    printf("test_hashmap_basic: OK\n");
}

int main(void) {
    test_hashmap_basic();
    return 0;
}
