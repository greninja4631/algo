#include "../include/data_structures.h"
#include "hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>   // ★追加
#include <string.h>   // strdupもここ

static void free_value(void* v) { free(v); }

void test_hashmap_basic() {
    hashmap_t* map = hashmap_create(8, free, free_value);

    assert(map);
    assert(hashmap_put(map, "hello", strdup("world")) == HM_OK);

    void* val = NULL;
    assert(hashmap_get(map, "hello", &val) == HM_OK);
    assert(strcmp((char*)val, "world") == 0);

    assert(hashmap_remove(map, "hello") == HM_OK);
    hashmap_destroy(map);
    printf("test_hashmap_basic: OK\n");
}

int main(void) {
    test_hashmap_basic();
    return 0;
}