#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include "data_structures.h"   // ds_error_t などを使うため

#ifdef __cplusplus
extern "C" {
#endif

// テスト用コマンド構造体
typedef struct {
    int op_code;
    int value;
} test_command_t;

// テスト用関数プロトタイプ
ds_error_t test_command_apply(void *ctx);
ds_error_t test_command_undo(void *ctx);

// // テストスイート宣言もここでOK
// void test__util_basic(void);
// void test__util_edge_cases(void);

#ifdef __cplusplus
}
#endif

#endif // TEST_UTIL_H
