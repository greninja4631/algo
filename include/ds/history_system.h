// include/ds/history_system.h
#ifndef DS_HISTORY_SYSTEM_H
#define DS_HISTORY_SYSTEM_H

#include <stddef.h>
#include <stdbool.h>
#include "data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif


// API宣言だけ
ds_history_system_t* ds_history_system_create(size_t max_history);
ds_error_t ds_history_system_destroy(ds_history_system_t* history);
ds_error_t ds_history_system_execute_command(ds_history_system_t* history, const ds_command_t* command);
ds_error_t ds_history_system_undo(ds_history_system_t* history);
ds_error_t ds_history_system_redo(ds_history_system_t* history);
bool ds_history_system_can_undo(const ds_history_system_t* history);
bool ds_history_system_can_redo(const ds_history_system_t* history);
ds_error_t ds_history_system_clear(ds_history_system_t* history);

#ifdef __cplusplus
}
#endif

#endif // DS_HISTORY_SYSTEM_H