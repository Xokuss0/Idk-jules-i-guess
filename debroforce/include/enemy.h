#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

#define MAX_ENEMIES 32

typedef struct {
    Entity entity;
    bool active;
    float attack_timer;
} Enemy;

void enemies_init(void);
void enemies_spawn(float x, float y);
void enemies_update(Map* map, float dt, float player_x);
void enemies_draw(void);

#endif
