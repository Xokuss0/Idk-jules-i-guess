#ifndef ASSETS_H
#define ASSETS_H

#include "hal/hal.h"

typedef enum {
    AT_RAMBRO,
    HAL_AT_BROMINATOR, // Avoid collision with existing BRO_BROMINATOR if any
    AT_BLADE,
    AT_MOOK,
    AT_GRASS,
    AT_DIRT,
    AT_EAGLE,
    AT_CAGE,
    AT_COUNT
} AssetID;

void assets_init(void);
void assets_exit(void);
Texture assets_get(AssetID id);

#endif
