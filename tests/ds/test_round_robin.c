#include "../include/ds/round_robin.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

<<<<<<< HEAD:tests/test_round_robin.c
int main(void) {
    printf("=== Round Robin Scheduler ADT Unit Test Start ===\n");
=======
#include "data_structures.h"
 #include <stdio.h>
 #include <assert.h>
 #include <string.h>
>>>>>>> feature:tests/ds/test_round_robin.c

    ds_round_robin_scheduler_t* scheduler = ds_round_robin_scheduler_create(10);
    assert(scheduler != NULL);

    ds_process_t p1 = {1, "proc-A", 5, 30, 1000};
    ds_process_t p2 = {2, "proc-B", 8, 20, 2000};
    ds_process_t p3 = {3, "proc-C", 3, 15, 3000};

    assert(ds_round_robin_scheduler_add_process(scheduler, &p1) == DS_SUCCESS);
    assert(ds_round_robin_scheduler_add_process(scheduler, &p2) == DS_SUCCESS);
    assert(ds_round_robin_scheduler_add_process(scheduler, &p3) == DS_SUCCESS);

    ds_process_t out;
    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id == 1);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id == 2);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id == 3);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id == 1);

    assert(ds_round_robin_scheduler_complete_process(scheduler, 2) == DS_SUCCESS);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_SUCCESS);
    assert(out.process_id != 2);

    // --- ここから異常系を全て ERR で
    assert(ds_round_robin_scheduler_add_process(NULL, &p1) == DS_ERR_NULL_POINTER);
    assert(ds_round_robin_scheduler_add_process(scheduler, NULL) == DS_ERR_NULL_POINTER);
    assert(ds_round_robin_scheduler_complete_process(scheduler, 99) == DS_ERR_NOT_FOUND);
    assert(ds_round_robin_scheduler_get_next_process(NULL, &out) == DS_ERR_NULL_POINTER);
    assert(ds_round_robin_scheduler_get_next_process(scheduler, NULL) == DS_ERR_NULL_POINTER);

    assert(ds_round_robin_scheduler_complete_process(scheduler, 1) == DS_SUCCESS);
    assert(ds_round_robin_scheduler_complete_process(scheduler, 3) == DS_SUCCESS);

    assert(ds_round_robin_scheduler_get_next_process(scheduler, &out) == DS_ERR_EMPTY_CONTAINER);

    assert(ds_round_robin_scheduler_destroy(scheduler) == DS_SUCCESS);

    printf("=== Round Robin Scheduler ADT Unit Test Passed ===\n");
    return 0;
}