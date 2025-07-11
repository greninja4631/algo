#ifndef NEXT_NEXT_PRACTICE_H
#define NEXT_NEXT_PRACTICE_H

#include <stdint.h>
#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

// ファクトリ関数（API例）
ds_next_next_practice_t* next_next_practice_create(void);
void next_next_practice_destroy(ds_next_next_practice_t* obj);

// テスト/デモ用
ds_error_t demonstrate_array_operations(void);
ds_error_t test_stack_operations(void);
ds_error_t test_queue_operations(void);
ds_error_t test_undo_redo_system(void);
ds_error_t test_circular_and_round_robin(void);

#ifdef __cplusplus
}
#endif
#endif // NEXT_NEXT_PRACTICE_H
