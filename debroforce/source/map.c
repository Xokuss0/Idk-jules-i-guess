#include <stddef.h>
#include "map.h"
#include "hal/hal.h"
#include "particles.h"
#include <math.h>

extern ParticleSystem g_ps;
static Texture tex_grass = NULL;

void map_init(Map* map) {
    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
        int y = i / MAP_WIDTH;
        if (y > 20) map->tiles[i] = TILE_DIRT;
        else if (y == 20) map->tiles[i] = TILE_GRASS;
        else map->tiles[i] = TILE_EMPTY;
    }
    tex_grass = hal_load_texture("gfx/grass.png");
}

void map_draw(Map* map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            TileType t = map->tiles[y * MAP_WIDTH + x];
            if (t == TILE_EMPTY) continue;

            if (t == TILE_GRASS && tex_grass) {
                hal_draw_texture(tex_grass, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, false);
            } else {
                Color c = COLOR_WHITE;
                if (t == TILE_DIRT) c = 0x8B4513FF;
                else if (t == TILE_GRASS) c = COLOR_GREEN;
                else if (t == TILE_STONE) c = 0x808080FF;

                hal_draw_rect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, c);
            }
        }
    }
}

void map_set_tile(Map* map, int x, int y, TileType type) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return;
    map->tiles[y * MAP_WIDTH + x] = type;
}

TileType map_get_tile(Map* map, int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return TILE_EMPTY;
    return map->tiles[y * MAP_WIDTH + x];
}

void map_explode(Map* map, int world_x, int world_y, int radius) {
    int tx = world_x / TILE_SIZE;
    int ty = world_y / TILE_SIZE;
    int tr = radius / TILE_SIZE;

    for (int dy = -tr; dy <= tr; dy++) {
        for (int dx = -tr; dx <= tr; dx++) {
            if (dx*dx + dy*dy <= tr*tr) {
                TileType t = map_get_tile(map, tx + dx, ty + dy);
                if (t != TILE_EMPTY) {
                    map_set_tile(map, tx + dx, ty + dy, TILE_EMPTY);
                    Color c = (t == TILE_DIRT) ? 0x8B4513FF : (t == TILE_GRASS ? COLOR_GREEN : 0x808080FF);
                    ps_spawn(&g_ps, (tx + dx) * TILE_SIZE + 8, (ty + dy) * TILE_SIZE + 8, c);
                }
            }
        }
    }
}
