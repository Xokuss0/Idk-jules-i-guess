#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <stdbool.h>

#define TILE_SIZE 16
#define MAP_WIDTH 64
#define MAP_HEIGHT 32

typedef enum {
    TILE_EMPTY = 0,
    TILE_DIRT = 1,
    TILE_GRASS = 2,
    TILE_STONE = 3,
    TILE_CAGE = 4
} TileType;

typedef struct {
    TileType tiles[MAP_WIDTH * MAP_HEIGHT];
} Map;

void map_init(Map* map);
void map_draw(Map* map);
void map_set_tile(Map* map, int x, int y, TileType type);
TileType map_get_tile(Map* map, int x, int y);
void map_explode(Map* map, int world_x, int world_y, int radius, void* ps);

#endif
