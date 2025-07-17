#include "algo/sort.h"
#include <stddef.h>

ds_error_t ds_sort_bubble(int* arr, size_t n) {
    if (!arr) return DS_ERR_NULL_POINTER;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j + 1 < n - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    return DS_SUCCESS;
}

ds_error_t ds_sort_insertion(int* arr, size_t n) {
    if (!arr) return DS_ERR_NULL_POINTER;
    for (size_t i = 1; i < n; ++i) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
    return DS_SUCCESS;
}
