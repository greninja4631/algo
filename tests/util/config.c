#include "util/config.h"
#include <util/test_config.h>
#include <stdlib.h>

struct ds_config {
    int dummy;   // ダミー本体。実プロダクトなら必要フィールドを書く
};

ds_config_t* ds_config_create(void) {
    return (ds_config_t*)malloc(sizeof(struct ds_config));
}

void ds_config_destroy(ds_config_t* config) {
    free(config);
}