#include "../include/data_structures.h"
#include <stdio.h>

int main() {
    ds_circular_list_t* list = ds_circular_list_create();
    int nums[] = {1,2,3};
    for(int i=0; i<3; ++i) ds_circular_list_insert(list, &nums[i]);

    void* out;
    while(!ds_circular_list_is_empty(list)) {
        ds_circular_list_remove(list, &out);
        printf("%d\n", *(int*)out);
    }
    ds_circular_list_destroy(list);
    return 0;
}
