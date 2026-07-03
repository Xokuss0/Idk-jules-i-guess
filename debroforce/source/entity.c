#include "entity.h"

bool entity_check_collision(Entity* e, Map* map, float dx, float dy) {
    float next_x = e->x + dx;
    float next_y = e->y + dy;

    int x1 = (next_x) / TILE_SIZE;
    int x2 = (next_x + e->w - 1) / TILE_SIZE;
    int y1 = (next_y) / TILE_SIZE;
    int y2 = (next_y + e->h - 1) / TILE_SIZE;

    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            if (map_get_tile(map, x, y) != TILE_EMPTY) {
                return true;
            }
        }
    }
    return false;
}

void entity_update(Entity* e, Map* map, float dt) {
    // Gravity
    e->vy += 800.0f * dt;

    // Horizontal Movement
    if (!entity_check_collision(e, map, e->vx * dt, 0)) {
        e->x += e->vx * dt;
    } else {
        e->vx = 0;
    }

    // Vertical Movement
    e->on_ground = false;
    if (!entity_check_collision(e, map, 0, e->vy * dt)) {
        e->y += e->vy * dt;
    } else {
        if (e->vy > 0) e->on_ground = true;
        e->vy = 0;
    }
}
