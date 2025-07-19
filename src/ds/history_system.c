/**
 * @file    src/ds/history_system.c
 * @brief   コマンド履歴管理システム – アロケータDI/安全設計/ガイドライン厳守
 * @details
 *   - すべての確保/解放はds_malloc/ds_free経由、直呼び完全禁止
 *   - alloc==NULLは禁止、呼び出し側で必ずDIする前提
 *   - Opaque設計、構造体本体は.cのみ
 */

#include "ds/history_system.h"
#include "util/memory.h"
#include "util/logger.h"
#include <stddef.h>
#include <string.h>

/*────────────────────────────
 * 内部構造体（Opaque本体）
 *────────────────────────────*/
typedef struct history_node {
    void *command;
    struct history_node *prev, *next;
} history_node_t;

struct ds_history_system {
    const ds_allocator_t *alloc;
    history_node_t *head, *tail, *current;
    size_t size, max_size;
};

/*────────────────────────────
 * 内部ユーティリティ
 *────────────────────────────*/
static inline void *xalloc(const ds_allocator_t *a, size_t n, size_t sz)
{
    // 禁止: a==NULLでのフォールバック
    if (!a) return NULL;
    return ds_malloc(a, n, sz);
}
static inline void xfree(const ds_allocator_t *a, void *p)
{
    if (a && p) ds_free(a, p);
}

/*────────────────────────────
 * 生成／破棄
 *────────────────────────────*/
ds_error_t ds_history_system_create(const ds_allocator_t *alloc, size_t max_size, ds_history_system_t **out_sys)
{
    if (!alloc || !out_sys) return DS_ERR_NULL_POINTER;
    ds_history_system_t *sys = xalloc(alloc, 1, sizeof *sys);
    if (!sys) return DS_ERR_ALLOC;
    memset(sys, 0, sizeof *sys);
    sys->alloc = alloc;
    sys->max_size = max_size;
    *out_sys = sys;
    return DS_SUCCESS;
}

ds_error_t ds_history_system_destroy(const ds_allocator_t *alloc, ds_history_system_t *sys)
{
    if (!sys) return DS_ERR_NULL_POINTER;
    history_node_t *cur = sys->head;
    while (cur) {
        history_node_t *next = cur->next;
        xfree(sys->alloc, cur->command);
        xfree(sys->alloc, cur);
        cur = next;
    }
    xfree(sys->alloc, sys);
    return DS_SUCCESS;
}

/*────────────────────────────
 * コマンド登録・操作系
 *────────────────────────────*/
ds_error_t ds_history_system_execute_command(const ds_allocator_t *alloc, ds_history_system_t *sys, void *cmd)
{
    if (!alloc || !sys || !cmd) return DS_ERR_NULL_POINTER;

    // ノード生成
    history_node_t *node = xalloc(sys->alloc, 1, sizeof *node);
    if (!node) return DS_ERR_ALLOC;
    node->command = cmd;
    node->prev = sys->tail;
    node->next = NULL;

    if (sys->tail)
        sys->tail->next = node;
    else
        sys->head = node;
    sys->tail = node;
    sys->current = node;
    sys->size++;

    // FIFO最大数超えたら先頭ノード削除
    if (sys->size > sys->max_size) {
        history_node_t *victim = sys->head;
        sys->head = victim->next;
        if (sys->head)
            sys->head->prev = NULL;
        xfree(sys->alloc, victim->command);
        xfree(sys->alloc, victim);
        sys->size--;
    }
    return DS_SUCCESS;
}

ds_error_t ds_history_system_undo(const ds_allocator_t *alloc, ds_history_system_t *sys)
{
    if (!alloc || !sys || !sys->current) return DS_ERR_INVALID_ARG;
    if (sys->current->prev)
        sys->current = sys->current->prev;
    return DS_SUCCESS;
}

ds_error_t ds_history_system_redo(const ds_allocator_t *alloc, ds_history_system_t *sys)
{
    if (!alloc || !sys || !sys->current) return DS_ERR_INVALID_ARG;
    if (sys->current->next)
        sys->current = sys->current->next;
    return DS_SUCCESS;
}

bool ds_history_system_can_undo(const ds_history_system_t *sys)
{
    return sys && sys->current && sys->current->prev;
}
bool ds_history_system_can_redo(const ds_history_system_t *sys)
{
    return sys && sys->current && sys->current->next;
}

/*────────────────────────────
 * クリア
 *────────────────────────────*/
ds_error_t ds_history_system_clear(const ds_allocator_t *alloc, ds_history_system_t *sys)
{
    if (!alloc || !sys) return DS_ERR_NULL_POINTER;
    history_node_t *cur = sys->head;
    while (cur) {
        history_node_t *next = cur->next;
        xfree(sys->alloc, cur->command);
        xfree(sys->alloc, cur);
        cur = next;
    }
    sys->head = sys->tail = sys->current = NULL;
    sys->size = 0;
    return DS_SUCCESS;
}