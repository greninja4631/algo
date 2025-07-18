#ifndef TEST_DS_TEST_CIRCULAR_LIST_H
#define TEST_DS_TEST_CIRCULAR_LIST_H

#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

// すべてのテスト関数の先頭引数に「const ds_allocator_t* alloc」を追加！
void test__circular_list_basic(const ds_allocator_t *alloc);
void test__circular_list_edge_cases(const ds_allocator_t *alloc);

#ifdef __cplusplus
}
#endif

#endif // TEST_DS_TEST_CIRCULAR_LIST_H