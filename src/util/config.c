#include "util/config.h"
#include <stddef.h>

/// Opaque本体は.c限定
struct ds_config {
    int dummy;  // 今後の拡張用（例：構成値など）
};

ds_error_t ds_config_create(const ds_allocator_t* alloc, ds_config_t **out_cfg)
{
    if (!alloc || !out_cfg)
        return DS_ERR_NULL_POINTER;

    *out_cfg = alloc->alloc(1, sizeof(ds_config_t));
    if (!*out_cfg)
        return DS_ERR_ALLOC;

    // 初期化（例：(*out_cfg)->dummy = 0;）
    (*out_cfg)->dummy = 0;
    return DS_SUCCESS;
}

void ds_config_destroy(const ds_allocator_t* alloc, ds_config_t *cfg)
{
    if (!alloc || !cfg) return;
    alloc->free(cfg);
}
