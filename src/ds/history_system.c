#include "ds/history_system.h"
#include "util/memory.h"
#include "util/logger.h"
#include <stddef.h>
#include <string.h>

/* 内部構造体定義 */
typedef struct history_node {
    const ds_command_t *command;  // constポインタ型で統一
    struct history_node *prev, *next;
} history_node_t;

struct ds_history_system {
    const ds_allocator_t *alloc;
    history_node_t *head, *tail, *current;
    size_t size, max_size;
};

/* ユーティリティ */
static inline void *xalloc(const ds_allocator_t *a, size_t n, size_t sz) {
    if (!a) return NULL;
    return ds_malloc(a, n, sz);
}
static inline void xfree(const ds_allocator_t *a, void *p) {
    if (a && p) ds_free(a, p);
}

/* 生成 */
ds_error_t ds_history_system_create(const ds_allocator_t *alloc, size_t max_size, ds_history_system_t **out_sys) {
    if (!alloc || !out_sys) return DS_ERR_NULL_POINTER;
    ds_history_system_t *sys = xalloc(alloc, 1, sizeof *sys);
    if (!sys) return DS_ERR_ALLOC;
    memset(sys, 0, sizeof *sys);
    sys->alloc = alloc;
    sys->max_size = max_size;
    *out_sys = sys;
    return DS_SUCCESS;
}

/* 破棄 */
ds_error_t ds_history_system_destroy(const ds_allocator_t *alloc, ds_history_system_t *sys) {
    (void)alloc; // unusedパラメータ警告対策
    if (!sys) return DS_ERR_NULL_POINTER;
    history_node_t *cur = sys->head;
    while (cur) {
        history_node_t *next = cur->next;
        // command解放は必要なら設計に応じて
        xfree(sys->alloc, (void*)cur->command);
        xfree(sys->alloc, cur);
        cur = next;
    }
    xfree(sys->alloc, sys);
    return DS_SUCCESS;
}

/* コマンド実行 */
ds_error_t ds_history_system_execute_command(
    const ds_allocator_t *alloc,
    ds_history_system_t  *history,
    const ds_command_t   *command)
{
    if (!alloc || !history || !command) return DS_ERR_NULL_POINTER;

    history_node_t *node = xalloc(history->alloc, 1, sizeof *node);
    if (!node) return DS_ERR_ALLOC;
    node->command = command; // const型で一致

    node->prev = history->tail;
    node->next = NULL;

    if (history->tail)
        history->tail->next = node;
    else
        history->head = node;
    history->tail = node;
    history->current = node;
    history->size++;

    // FIFO最大数超過時に先頭ノード削除
    if (history->max_size && history->size > history->max_size) {
        history_node_t *victim = history->head;
        history->head = victim->next;
        if (history->head)
            history->head->prev = NULL;
        xfree(history->alloc, (void*)victim->command);
        xfree(history->alloc, victim);
        history->size--;
    }
    return DS_SUCCESS;
}

ds_error_t ds_history_system_undo(const ds_allocator_t *alloc, ds_history_system_t *history) {
    if (!alloc || !history || !history->current) return DS_ERR_INVALID_ARG;
    if (history->current->prev)
        history->current = history->current->prev;
    return DS_SUCCESS;
}

ds_error_t ds_history_system_redo(const ds_allocator_t *alloc, ds_history_system_t *history) {
    if (!alloc || !history || !history->current) return DS_ERR_INVALID_ARG;
    if (history->current->next)
        history->current = history->current->next;
    return DS_SUCCESS;
}

bool ds_history_system_can_undo(const ds_history_system_t *history) {
    return history && history->current && history->current->prev;
}
bool ds_history_system_can_redo(const ds_history_system_t *history) {
    return history && history->current && history->current->next;
}

ds_error_t ds_history_system_clear(const ds_allocator_t *alloc, ds_history_system_t *history) {
    if (!alloc || !history) return DS_ERR_NULL_POINTER;
    history_node_t *cur = history->head;
    while (cur) {
        history_node_t *next = cur->next;
        xfree(history->alloc, (void*)cur->command);
        xfree(history->alloc, cur);
        cur = next;
    }
    history->head = history->tail = history->current = NULL;
    history->size = 0;
    return DS_SUCCESS;
}
