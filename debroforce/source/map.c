#include <stddef.h>
#include "map.h"
#include "hal/hal.h"
#include "particles.h"
#include "assets.h"
#include <math.h>
#include <stdlib.h>

void map_init(Map* map) {
    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
        int x = i % MAP_WIDTH;
        int y = i / MAP_WIDTH;
        int ground_y = 15 + (int)(3.0 * sin(x * 0.2));
        bool is_building = (x >= 10 && x <= 15 && y >= ground_y - 5 && y < ground_y);
        if (is_building) {
            map->tiles[i] = TILE_STONE;
        } else if (y > ground_y) {
            map->tiles[i] = TILE_DIRT;
        } else if (y == ground_y) {
            map->tiles[i] = TILE_GRASS;
        } else {
            map->tiles[i] = TILE_EMPTY;
        }
    }
}

void map_draw(Map* map) {
    // Terrain uses flat colors for maximum performance on Old 3DS
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            TileType t = map->tiles[y * MAP_WIDTH + x];
            if (t == TILE_EMPTY) continue;
            Color c = COLOR_WHITE;
            if (t == TILE_DIRT) c = 0x8B4513FF; // Earth Brown
            else if (t == TILE_GRASS) c = 0x228B22FF; // Grass Green
            else if (t == TILE_STONE) c = 0x696969FF; // Dim Gray
            hal_draw_rect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, c);
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

void map_explode(Map* map, int world_x, int world_y, int radius, void* ps) {
    ParticleSystem* particles = (ParticleSystem*)ps;
    int tx = world_x / TILE_SIZE;
    int ty = world_y / TILE_SIZE;
    int tr = radius / TILE_SIZE;
    for (int dy = -tr; dy <= tr; dy++) {
        for (int dx = -tr; dx <= tr; dx++) {
            if (dx*dx + dy*dy <= (tr+0.5)*(tr+0.5)) {
                TileType t = map_get_tile(map, tx + dx, ty + dy);
                if (t != TILE_EMPTY) {
                    map_set_tile(map, tx + dx, ty + dy, TILE_EMPTY);
                    Color c = (t == TILE_DIRT) ? 0x8B4513FF : (t == TILE_GRASS ? 0x228B22FF : 0x696969FF);
                    if (particles) ps_spawn(particles, (tx + dx) * TILE_SIZE + 8, (ty + dy) * TILE_SIZE + 8, c);
                }
            }
        }
    }
}
