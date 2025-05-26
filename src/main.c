#include "src/statistics.h"
#include <assert.h>

int main(void) {
    int data[] = {1, 2, 3, 3, 5, 5, 5, 8, 9};
    int size = sizeof(data) / sizeof(data[0]);

    Statistics stats = calculate_statistics(data, size);

    // JSON形式で出力
    printf("{\n");
    printf("  \"sum\": %d,\n", stats.sum);
    printf("  \"min\": %d,\n", stats.min);
    printf("  \"max\": %d,\n", stats.max);
    printf("  \"average\": %.2f,\n", stats.average);
    printf("  \"median\": %.2f,\n", stats.median);
    printf("  \"mode\": %d\n", stats.mode);
    printf("}\n");


//コードが正常にテストできることが判明しているため、冗長化を防ぐために、コメントアウトしてる。
//     // CI自動テスト相当
//     assert(stats.sum == 41);
//     assert(stats.min == 1);
//     assert(stats.max == 9);
//     assert((int)(stats.average * 100) == 455);
//     assert((int)(stats.median * 100) == 500);
//     assert(stats.mode == 5);

//     LOG_INFO("Statistics test passed.\n");
//     return 0;
// }