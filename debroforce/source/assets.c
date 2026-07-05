#include "assets.h"
#include <stddef.h>

static Texture g_textures[AT_COUNT];

static const char* g_asset_paths[AT_COUNT] = {
    [AT_RAMBRO] = "gfx/rambro.png",
    [HAL_AT_BROMINATOR] = "gfx/brominator.png",
    [AT_BLADE] = "gfx/blade.png",
    [AT_MOOK] = "gfx/mook.png",
    [AT_GRASS] = "gfx/grass.png",
    [AT_DIRT] = "gfx/dirt.png", // We'll create this or use grass as fallback
    [AT_EAGLE] = "gfx/eagle.png",
    [AT_CAGE] = "gfx/cage.png"
};

void assets_init(void) {
    for (int i = 0; i < AT_COUNT; i++) {
        if (g_asset_paths[i]) {
            g_textures[i] = hal_load_texture(g_asset_paths[i]);
        } else {
            g_textures[i] = NULL;
        }
    }
}

void assets_exit(void) {
    // Ideally HAL would have a texture free function
}

Texture assets_get(AssetID id) {
    if (id < 0 || id >= AT_COUNT) return NULL;
    return g_textures[id];
}
