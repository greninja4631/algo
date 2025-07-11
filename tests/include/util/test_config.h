#ifndef DS_CONFIG_H
#define DS_CONFIG_H

// #include "util/config.h"

#ifdef __cplusplus
extern "C" {
#endif


// API宣言
ds_config_t* ds_config_create(void);
void ds_config_destroy(ds_config_t* config);

#ifdef __cplusplus
}
#endif

#endif // DS_CONFIG_H