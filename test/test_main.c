#include "src/statistics.h"
#include <assert.h>
#include <stdio.h>

/**
 * @file test_main.c
 * @brief ユニットテスト（main以外全APIを検証）
 * [設計理由]
 * - JenkinsやGitHub Actionsで自動実行・失敗時はCIで通知
 * - main重複を避け-DTESTING条件下のみビルド
 */

int main(void) {
    int data[] = {1, 2, 3, 3, 5, 5, 5, 8, 9};
    int size = sizeof(data) / sizeof(data[0]);

    //単体テストを行うコード
    //単体の関数のみ問題なく動くかどうかをテストする
    assert(sum(data, size) == 41);
    assert(min(data, size) == 1);
    assert(max(data, size) == 9);
    assert((int)(average(data, size) * 100) == 455);
    assert((int)(median(data, size) * 100) == 500);
    assert(mode(data, size) == 5);


     //複合テストを行うコード
    //関数同士の連携で問題なく作動するかどうかをテストする
    Statistics stats = calculate_statistics(data, size);
    assert(stats.sum == 41);
    assert(stats.min == 1);
    assert(stats.max == 9);
    assert((int)(stats.average * 100) == 455);
    assert((int)(stats.median * 100) == 500);
    assert(stats.mode == 5);

    printf("[TEST] All assertions passed!\n");
    return 0;
}