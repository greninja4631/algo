#include "util/config.h"
#include <stdlib.h>

struct ds_config {
  int dummy __attribute__((unused));
};

ds_error_t ds_config_create(ds_config_t **out_cfg)
{
    if (!out_cfg) 
        return DS_ERR_NULL_POINTER;
    *out_cfg = calloc(1, sizeof(ds_config_t));
    return *out_cfg ? DS_SUCCESS : DS_ERR_ALLOC;
}

void ds_config_destroy(ds_config_t *cfg)
{
    free(cfg);
}