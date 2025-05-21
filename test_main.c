// test_main.c
#include <assert.h>
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    assert(add(2, 3) == 5);
    assert(add(-1, 1) == 0);
    printf("[TEST] Cテスト成功\n");
    return 0;
}