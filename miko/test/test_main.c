#include <assert.h>
#include <stdio.h>
#include "../statistics.c"  // または "statistics.c"（ビルド環境によって相対パスを調整）

int main(void) {
    int data[] = {1, 2, 3, 3, 5, 5, 5, 8, 9};
    int size = sizeof(data) / sizeof(data[0]);

    // --- 個別関数のテスト ---
    assert(sum(data, size) == 41);         // 合計
    assert(min(data, size) == 1);          // 最小値
    assert(max(data, size) == 9);          // 最大値
    assert((int)(average(data, size) * 100) == 456); // 平均値（小数点対策）
    assert((int)(median(data, size) * 100) == 500);  // 中央値
    assert(mode(data, size) == 5);         // 最頻値

    // --- 構造体経由の総合テスト ---
    Statistics stats = {
        .sum     = sum(data, size),
        .min     = min(data, size),
        .max     = max(data, size),
        .average = average(data, size),
        .median  = median(data, size),
        .mode    = mode(data, size)
    };

    assert(stats.sum == 41);
    assert(stats.min == 1);
    assert(stats.max == 9);
    assert((int)(stats.average * 100) == 456);
    assert((int)(stats.median * 100) == 500);
    assert(stats.mode == 5);

    printf("[TEST] All assertions passed!\n");
    return 0;
}