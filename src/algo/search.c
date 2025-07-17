#include "algo/search.h"
#include <stddef.h>

ds_error_t ds_search_linear(const int* arr, size_t n, int target, ptrdiff_t* out_index) {
    if (!arr || !out_index) return DS_ERR_NULL_POINTER;
    for (size_t i = 0; i < n; ++i) {
        if (arr[i] == target) {
            *out_index = (ptrdiff_t)i;
            return DS_SUCCESS;
        }
    }
    *out_index = -1;
    return DS_ERR_NOT_FOUND;
}

ds_error_t ds_search_binary(const int* arr, size_t n, int target, ptrdiff_t* out_index) {
    if (!arr || !out_index) return DS_ERR_NULL_POINTER;
    size_t left = 0, right = n;
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            *out_index = (ptrdiff_t)mid;
            return DS_SUCCESS;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    *out_index = -1;
    return DS_ERR_NOT_FOUND;
}
