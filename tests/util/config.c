#include "util/config.h"
#include <stddef.h>  // NULL チェック用

// --- 構造体本体は .c 限定（Opaque型原則）---
struct ds_config {
    int dummy; // 実際の設定値をここに追加
};

// --- API実装 ---

ds_error_t ds_config_create(const ds_allocator_t* alloc, ds_config_t **out_cfg)
{
    if (!alloc || !out_cfg)
        return DS_ERR_NULL_POINTER;

    *out_cfg = alloc->alloc(1, sizeof(ds_config_t));
    if (!*out_cfg)
        return DS_ERR_ALLOC;

    // 必要に応じて初期値セット
    (*out_cfg)->dummy = 0;

    return DS_SUCCESS;
}

void ds_config_destroy(const ds_allocator_t* alloc, ds_config_t *cfg)
{
    if (!alloc || !cfg) return;
    alloc->free(cfg);
}
