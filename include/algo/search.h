#ifndef DS_ALGO_SEARCH_H
#define DS_ALGO_SEARCH_H

#include <stddef.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

ds_error_t ds_search_linear(const int* arr, size_t n, int target, ptrdiff_t* out_index);
ds_error_t ds_search_binary(const int* arr, size_t n, int target, ptrdiff_t* out_index);

#ifdef __cplusplus
}
#endif

#endif // DS_ALGO_SEARCH_H
