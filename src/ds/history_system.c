// src/ds/history_system.c
#include "ds/history_system.h"
#include <stdlib.h>
#include <string.h>

/*──────────────────────────────────────────*/
/* 内部クリア関数プロトタイプ（static）    */
/*──────────────────────────────────────────*/
static void static_clear_history(ds_history_system_t *hist);

/*──────────────────────────────────────────*/
/* 内部構造                                */
/*──────────────────────────────────────────*/
typedef struct history_node {
    ds_command_t        *command;
    struct history_node *next;
    struct history_node *prev;
} history_node_t;

struct ds_history_system {
    history_node_t *head;
    history_node_t *tail;
    history_node_t *current;
    size_t          max_history;
};

/*──────────────────────────────────────────*/
/* ノード生成ユーティリティ（static）      */
/*──────────────────────────────────────────*/
static history_node_t *make_node(const ds_command_t *cmd) {
    if (!cmd) return NULL;
    history_node_t *n = malloc(sizeof *n);
    if (!n) return NULL;
    n->command = (ds_command_t *)cmd;  /* shallow copy */
    n->next = n->prev = NULL;
    return n;
}

/*──────────────────────────────────────────*/
/* API 実装                                */
/*──────────────────────────────────────────*/
ds_history_system_t *
ds_history_system_create(size_t max_history)
{
    ds_history_system_t *h = calloc(1, sizeof *h);
    if (!h) return NULL;
    h->max_history = max_history ? max_history : SIZE_MAX;
    return h;
}

ds_error_t
ds_history_system_destroy(ds_history_system_t *hist)
{
    if (!hist) return DS_ERR_NULL_POINTER;
    /* 内部ノードをすべて解放 */
    static_clear_history(hist);
    free(hist);
    return DS_SUCCESS;
}

ds_error_t
ds_history_system_execute_command(ds_history_system_t *hist,
                                  const ds_command_t  *command)
{
    if (!hist || !command || !command->execute)
        return DS_ERR_NULL_POINTER;

    /* コマンド実行 */
    ds_error_t exec_result = command->execute(command->ctx);
    if (exec_result != DS_SUCCESS)
        return exec_result;

    /* 成功したら履歴にノード追加 */
    history_node_t *node = make_node(command);
    if (!node)
        return DS_ERR_ALLOC;

    node->prev = hist->tail;
    if (hist->tail) hist->tail->next = node;
    else            hist->head       = node;
    hist->tail    = node;
    hist->current = node;

    /* max_history を超えたら先頭から古いノード削除 */
    size_t count = 0;
    for (history_node_t *p = hist->tail; p; p = p->prev) {
        if (++count > hist->max_history && p->prev) {
            history_node_t *dead = p->prev;
            dead->prev->next = NULL;
            free(dead);
            hist->head = p;
            break;
        }
    }
    return DS_SUCCESS;
}

bool
ds_history_system_can_undo(const ds_history_system_t *hist)
{
    return hist && hist->current;
}

bool
ds_history_system_can_redo(const ds_history_system_t *hist)
{
    return hist && hist->current && hist->current->next;
}

ds_error_t
ds_history_system_undo(ds_history_system_t *hist)
{
    if (!hist)               return DS_ERR_NULL_POINTER;
    if (!hist->current)      return DS_ERR_EMPTY;

    ds_error_t res = hist->current->command->undo(
                        hist->current->command->ctx);
    if (res != DS_SUCCESS)   return res;

    hist->current = hist->current->prev;
    return DS_SUCCESS;
}

ds_error_t
ds_history_system_redo(ds_history_system_t *hist)
{
    if (!hist)                       return DS_ERR_NULL_POINTER;
    if (!hist->current || !hist->current->next)
        return DS_ERR_EMPTY;

    history_node_t *next = hist->current->next;
    if (!next->command->execute)
        return DS_ERR_SYSTEM;

    ds_error_t res = next->command->execute(next->command->ctx);
    if (res == DS_SUCCESS)
        hist->current = next;
    return res;
}

/*──────────────────────────────────────────*/
/* 内部クリア関数（static／戻り値 void）  */
/*──────────────────────────────────────────*/
static void
static_clear_history(ds_history_system_t *hist)
{
    history_node_t *p = hist->head;
    while (p) {
        history_node_t *nx = p->next;
        free(p);
        p = nx;
    }
    hist->head    = NULL;
    hist->tail    = NULL;
    hist->current = NULL;
}