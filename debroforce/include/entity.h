#ifndef ENTITY_H
#define ENTITY_H

#include "map.h"

typedef struct {
    float x, y;
    float vx, vy;
    float w, h;
    bool on_ground;
    int facing; // 1 for right, -1 for left
} Entity;

void entity_update(Entity* e, Map* map, float dt);
bool entity_check_collision(Entity* e, Map* map, float dx, float dy);

#endif
