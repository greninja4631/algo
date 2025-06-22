#include "../include/data_structures.h"
#include <stdio.h>
#include <assert.h>


int main(void) {
    printf("=== Doubly List ADT Unit Test Start ===\n");

    // 1. リスト生成
    ds_doubly_list_t* list = ds_doubly_list_create();
    assert(list != NULL);

    // 2. 初期状態の確認
    assert(ds_doubly_list_is_empty(list) == true);
    assert(ds_doubly_list_size(list) == 0);

    // 3. 先頭/末尾/指定位置への挿入
    int a = 10, b = 20, c = 30, d = 40, e = 50;
    assert(ds_doubly_list_insert_front(list, &a) == DS_SUCCESS); // [10]
    assert(ds_doubly_list_insert_back(list, &b)  == DS_SUCCESS); // [10, 20]
    assert(ds_doubly_list_insert_at(list, 1, &c) == DS_SUCCESS); // [10, 30, 20]
    assert(ds_doubly_list_insert_back(list, &d)  == DS_SUCCESS); // [10, 30, 20, 40]
    assert(ds_doubly_list_insert_front(list, &e) == DS_SUCCESS); // [50, 10, 30, 20, 40]
    assert(ds_doubly_list_size(list) == 5);

    // 4. 指定位置の要素取得
    void* ptr = NULL;
    assert(ds_doubly_list_get_at(list, 0, &ptr) == DS_SUCCESS); // 先頭
    assert(*(int*)ptr == 50);
    assert(ds_doubly_list_get_at(list, 2, &ptr) == DS_SUCCESS); // 3番目
    assert(*(int*)ptr == 30);

    // 5. 先頭/末尾/任意位置の削除
    assert(ds_doubly_list_remove_front(list, &ptr) == DS_SUCCESS); // [10, 30, 20, 40]
    assert(*(int*)ptr == 50);
    assert(ds_doubly_list_remove_back(list, &ptr) == DS_SUCCESS);  // [10, 30, 20]
    assert(*(int*)ptr == 40);
    assert(ds_doubly_list_remove_at(list, 1, &ptr) == DS_SUCCESS); // [10, 20]
    assert(*(int*)ptr == 30);
    assert(ds_doubly_list_size(list) == 2);

    // 6. 残りの要素も削除
    assert(ds_doubly_list_remove_front(list, &ptr) == DS_SUCCESS); // [20]
    assert(*(int*)ptr == 10);
    assert(ds_doubly_list_remove_back(list, &ptr) == DS_SUCCESS);  // []
    assert(*(int*)ptr == 20);
    assert(ds_doubly_list_is_empty(list) == true);

    // 7. 空時のエラー系
    assert(ds_doubly_list_remove_front(list, &ptr) == DS_ERR_EMPTY_CONTAINER);
    assert(ds_doubly_list_get_at(list, 0, &ptr) == DS_ERR_NOT_FOUND);

    // 8. 統計情報取得
    ds_stats_t stats = {0};
    assert(ds_doubly_list_get_stats(list, &stats) == DS_SUCCESS);
    printf("Total elements (should be 0): %zu\n", stats.total_elements);

    // 9. リスト破棄
    assert(ds_doubly_list_destroy(list) == DS_SUCCESS);

    printf("=== Doubly List ADT Unit Test Passed ===\n");
    return 0;
}

// typedef struct history_node {
//     ds_command_t command;
//     struct history_node* prev;「前のページ（prev）」
//     struct history_node* next;「次のページ（next）」
// } history_node_t;

// struct ds_history_system {
//     history_node_t* head;      // 最古の履歴ノードへのポインタ（リストの先頭）
//     history_node_t* tail;      // 最新の履歴ノードへのポインタ（リストの末尾）
//     history_node_t* current;   // 現在位置（Undo/Redo判定用）、履歴リスト内の今のノード
//     size_t size;               // 履歴ノードの総数
//     size_t max_history;        // 保存できる履歴の最大数
// };

