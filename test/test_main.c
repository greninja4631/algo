#include "statistics.h"
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

    assert(sum(data, size) == 41);
    assert(min(data, size) == 1);
    assert(max(data, size) == 9);
    assert((int)(average(data, size) * 100) == 456);
    assert((int)(median(data, size) * 100) == 500);
    assert(mode(data, size) == 5);

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