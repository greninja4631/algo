<<<<<<< HEAD
#include "logger.h"
<<<<<<< HEAD
#include "../../include/data_structures.h"
#include "../../include/hashmap.h"
#include <stdlib.h>
#include <string.h>

// --- 定数 ---
#define DS_HASHMAP_INIT_BUCKETS 64

// --- 内部エントリ型 ---
typedef struct ds_hashmap_entry {
    char* key;
    void* value;
    struct ds_hashmap_entry* next;
} ds_hashmap_entry_t;

// --- 本体構造体 ---
struct ds_hashmap {
    ds_hashmap_entry_t** buckets;
    size_t num_buckets;
    size_t size;
    ds_hashmap_free_func key_free;
    ds_hashmap_free_func val_free;
};

// --- 内部: ハッシュ関数 ---
static unsigned long ds_hashmap_hash(const char* str) {
    unsigned long h = 5381;
    int c;
    while ((c = *str++))
        h = ((h << 5) + h) + c;
    return h;
}

// --- 内部: 新規エントリ作成 ---
static ds_hashmap_entry_t* ds_hashmap_new_entry(const char* key, void* value) {
    ds_hashmap_entry_t* e = (ds_hashmap_entry_t*)ds_malloc(sizeof(ds_hashmap_entry_t));
    if (!e) {
        ds_log(DS_LOG_ERROR, "[ds_hashmap] エントリ確保失敗\n");
        return NULL;
    }
    e->key = strdup(key);
    if (!e->key) {
        ds_free(e);
        ds_log(DS_LOG_ERROR, "[ds_hashmap] キー確保失敗\n");
        return NULL;
    }
    e->value = value;
    e->next = NULL;
    return e;
}

// --- API実装 ---

// 作成
ds_hashmap_t* ds_hashmap_create(size_t capacity, ds_hashmap_free_func key_free, ds_hashmap_free_func val_free) {
    ds_hashmap_t* map = (ds_hashmap_t*)ds_malloc(sizeof(ds_hashmap_t));
    if (!map) {
        ds_log(DS_LOG_ERROR, "[ds_hashmap] 本体確保失敗\n");
        return NULL;
    }
    size_t buckets = (capacity > 0) ? capacity : DS_HASHMAP_INIT_BUCKETS;
    map->buckets = (ds_hashmap_entry_t**)ds_malloc(sizeof(ds_hashmap_entry_t*) * buckets);
    if (!map->buckets) {
        ds_free(map);
        ds_log(DS_LOG_ERROR, "[ds_hashmap] バケット配列確保失敗\n");
        return NULL;
    }
    for (size_t i = 0; i < buckets; ++i) map->buckets[i] = NULL;
    map->num_buckets = buckets;
    map->size = 0;
    map->key_free = key_free;
    map->val_free = val_free;
    return map;
}

// 破棄
ds_error_t ds_hashmap_destroy(ds_hashmap_t* map) {
    if (!map) return DS_ERR_NULL_POINTER;
    for (size_t i = 0; i < map->num_buckets; ++i) {
        ds_hashmap_entry_t* e = map->buckets[i];
        while (e) {
            ds_hashmap_entry_t* next = e->next;
            if (map->key_free) map->key_free(e->key);
            if (map->val_free) map->val_free(e->value);
            ds_free(e);
            e = next;
        }
    }
    ds_free(map->buckets);
    ds_free(map);
    return DS_SUCCESS;
}

// put
ds_error_t ds_hashmap_put(ds_hashmap_t* map, const char* key, void* value) {
    if (!map || !key) return DS_ERR_NULL_POINTER;
    unsigned long idx = ds_hashmap_hash(key) % map->num_buckets;
    ds_hashmap_entry_t* e = map->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) {
            if (map->val_free) map->val_free(e->value);
            e->value = value;
            return DS_SUCCESS;
        }
        e = e->next;
    }
    // 新規
    e = ds_hashmap_new_entry(key, value);
    if (!e) return DS_ERR_ALLOC;
    e->next = map->buckets[idx];
    map->buckets[idx] = e;
    map->size++;
    return DS_SUCCESS;
}

// get
ds_error_t ds_hashmap_get(ds_hashmap_t* map, const char* key, void** out_value) {
    if (!map || !key || !out_value) return DS_ERR_NULL_POINTER;
    unsigned long idx = ds_hashmap_hash(key) % map->num_buckets;
    ds_hashmap_entry_t* e = map->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) {
            *out_value = e->value;
            return DS_SUCCESS;
        }
        e = e->next;
    }
    *out_value = NULL;
    return DS_ERR_NOT_FOUND;
}

// remove
ds_error_t ds_hashmap_remove(ds_hashmap_t* map, const char* key) {
    if (!map || !key) return DS_ERR_NULL_POINTER;
    unsigned long idx = ds_hashmap_hash(key) % map->num_buckets;
    ds_hashmap_entry_t** prev = &map->buckets[idx];
    ds_hashmap_entry_t* e = map->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) {
            *prev = e->next;
            if (map->key_free) map->key_free(e->key);
            if (map->val_free) map->val_free(e->value);
            ds_free(e);
            map->size--;
            return DS_SUCCESS;
        }
        prev = &e->next;
        e = e->next;
    }
    return DS_ERR_NOT_FOUND;
}

// サイズ
size_t ds_hashmap_size(const ds_hashmap_t* map) {
    return map ? map->size : 0;
}
=======
#include <stdbool.h>
#include <stdio.h>
/**
 * @file history_system.c
 * @brief Undo/Redo履歴システム ADT 実装
 * @version 1.0.0
 * @date 2025-05-28
 *
 * 設計方針:
 * - コマンドパターン+双方向リスト/スタックでUndo/Redoの履歴を管理
 * - ADTで外部から内部状態を隠蔽
 * - エラー処理・テスト容易性・依存性注入・情報隠蔽を徹底
 */

#include "data_structures.h"
 #include <stdlib.h>
 #include <string.h>
 
 /* ---- 内部構造体定義 ---- */
 typedef struct history_node {
     ds_command_t command;
     struct history_node* prev;
     struct history_node* next;
 } history_node_t;
 
 struct ds_history_system {
     history_node_t* head;
     history_node_t* tail;
     history_node_t* current;
     size_t size;
     size_t max_history;
 };
 
 /* ---- 内部ユーティリティ ---- */
 static void clone_command(ds_command_t* dest, const ds_command_t* src) {
     memcpy(dest, src, sizeof(ds_command_t));
     // 必要に応じてdataの中身（deep copy）が必要な場合はカスタマイズする
 }
 static void free_node(history_node_t* node) {
     if (!node) return;
     // 必要に応じてnode->command.dataのfreeを呼ぶ
     free(node);
 }
 
 /* ---- API実装 ---- */
 
 ds_history_system_t* ds_history_system_create(size_t max_history) {
     ds_history_system_t* sys = (ds_history_system_t*)malloc(sizeof(ds_history_system_t));
     if (!sys) return NULL;
     sys->head = sys->tail = sys->current = NULL;
     sys->size = 0;
     sys->max_history = max_history;
     return sys;
 }
 
 ds_error_t ds_history_system_destroy(ds_history_system_t* history) {
     if (!history) return DS_ERR_NULL_POINTER;
     history_node_t* node = history->head;
     while (node) {
         history_node_t* next = node->next;
         free_node(node);
         node = next;
     }
     free(history);
     return DS_SUCCESS;
 }
 
 ds_error_t ds_history_system_execute_command(ds_history_system_t* history, const ds_command_t* command) {
     if (!history || !command || !command->execute) return DS_ERR_NULL_POINTER;
 
     // Redo履歴を破棄
     history_node_t* node = history->current ? history->current->next : NULL;
     while (node) {
         history_node_t* next = node->next;
         free_node(node);
         node = next;
         history->size--;
     }
     if (history->current) history->current->next = NULL;
     history->tail = history->current;
 
     // コマンド実行
     ds_error_t exec_result = command->execute(command->data);
     if (exec_result != DS_SUCCESS) return exec_result;
 
     // 新ノード作成
     history_node_t* new_node = (history_node_t*)malloc(sizeof(history_node_t));
     if (!new_node) return DS_ERR_OUT_OF_MEMORY;
     clone_command(&new_node->command, command);
     new_node->prev = history->current;
     new_node->next = NULL;
     if (history->current) history->current->next = new_node;
     history->current = new_node;
     if (!history->head) history->head = new_node;
     history->tail = new_node;
     history->size++;
 
     // 最大履歴超過時は古いものから削除
     while (history->max_history > 0 && history->size > history->max_history) {
         history_node_t* old_head = history->head;
         history->head = old_head->next;
         if (history->head) history->head->prev = NULL;
         free_node(old_head);
         history->size--;
     }
 
     return DS_SUCCESS;
 }
 
 ds_error_t ds_history_system_undo(ds_history_system_t* history) {
     if (!history || !history->current) return DS_ERR_INVALID_ARG;
     if (!history->current->command.undo) return DS_ERR_SYSTEM_FAILURE;
     ds_error_t result = history->current->command.undo(history->current->command.data);
     if (result != DS_SUCCESS) return result;
     history->current = history->current->prev;
     return DS_SUCCESS;
 }
 
 ds_error_t ds_history_system_redo(ds_history_system_t* history) {
     if (!history || !history->current || !history->current->next)
         return DS_ERR_INVALID_ARG;
     history_node_t* next = history->current->next;
     if (!next->command.execute) return DS_ERR_SYSTEM_FAILURE;
     ds_error_t result = next->command.execute(next->command.data);
     if (result != DS_SUCCESS) return result;
     history->current = next;
     return DS_SUCCESS;
 }
 
 bool ds_history_system_can_undo(const ds_history_system_t* history) {
     return history && history->current && history->current->prev;
 }
 
 bool ds_history_system_can_redo(const ds_history_system_t* history) {
     return history && history->current && history->current->next;
 }
 
 ds_error_t ds_history_system_clear(ds_history_system_t* history) {
     if (!history) return DS_ERR_NULL_POINTER;
     history_node_t* node = history->head;
     while (node) {
         history_node_t* next = node->next;
         free_node(node);
         node = next;
     }
     history->head = history->tail = history->current = NULL;
     history->size = 0;
     return DS_SUCCESS;
 }
>>>>>>> feature
=======
// src/ds/history_system.c
#include "util/memory.h"
#include "ds/history_system.h"

#include <stdlib.h>
#include <string.h>

/*──────────────────────────────*/
/* 内部クリア関数プロトタイプ    */
/*──────────────────────────────*/
static void static_clear_history(const ds_allocator_t* alloc, ds_history_system_t *hist);

/*──────────────────────────────*/
/* 内部構造                      */
/*──────────────────────────────*/
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
    const ds_allocator_t *alloc;
};

/*──────────────────────────────*/
/* ノード生成ユーティリティ      */
/*──────────────────────────────*/
static history_node_t *make_node(const ds_allocator_t *alloc, const ds_command_t *cmd) {
    if (!cmd) return NULL;
    history_node_t *n = ds_malloc(alloc, 1, sizeof *n);
    if (!n) return NULL;
    n->command = (ds_command_t *)cmd;  /* shallow copy */
    n->next = n->prev = NULL;
    return n;
}

/*──────────────────────────────*/
/* API 実装                      */
/*──────────────────────────────*/
ds_history_system_t *
ds_history_system_create(const ds_allocator_t* alloc, size_t max_history)
{
    ds_history_system_t *h = ds_malloc(alloc, 1, sizeof *h);
    if (!h) return NULL;
    h->max_history = max_history ? max_history : SIZE_MAX;
    h->alloc = alloc;
    h->head = h->tail = h->current = NULL;
    return h;
}

ds_error_t
ds_history_system_destroy(const ds_allocator_t* alloc, ds_history_system_t *hist)
{
    if (!hist) return DS_ERR_NULL_POINTER;
    /* 内部ノードをすべて解放 */
    static_clear_history(alloc, hist);
    ds_free(alloc, hist);
    return DS_SUCCESS;
}

ds_error_t
ds_history_system_execute_command(ds_history_system_t *hist, const ds_command_t  *command)
{
    if (!hist || !command || !command->execute)
        return DS_ERR_NULL_POINTER;

    const ds_allocator_t* alloc = hist->alloc;

    /* コマンド実行 */
    ds_error_t exec_result = command->execute(command->ctx);
    if (exec_result != DS_SUCCESS)
        return exec_result;

    /* 成功したら履歴にノード追加 */
    history_node_t *node = make_node(alloc, command);
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
            if (dead->prev) dead->prev->next = NULL;
            ds_free(alloc, dead);
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

/*──────────────────────────────*/
/* 内部クリア関数               */
/*──────────────────────────────*/
static void
static_clear_history(const ds_allocator_t* alloc, ds_history_system_t *hist)
{
    history_node_t *p = hist->head;
    while (p) {
        history_node_t *nx = p->next;
        ds_free(alloc, p);
        p = nx;
    }
<<<<<<< HEAD
    hist->head = hist->tail = hist->current = NULL;
    return DS_SUCCESS;
}
>>>>>>> feature
=======
    hist->head    = NULL;
    hist->tail    = NULL;
    hist->current = NULL;
}
>>>>>>> feature
