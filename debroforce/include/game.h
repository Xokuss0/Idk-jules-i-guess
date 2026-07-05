#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "bro.h"
#include "map.h"
#include "particles.h"
#include "enemy.h"
#include "rescue.h"

typedef enum {
    STATE_MENU,
    STATE_PLAYING
} GameStateID;

typedef struct {
    GameStateID state;
    Map map;
    ParticleSystem ps;
    Bro current_bro;
    int camera_x, camera_y;
    bool running;
} Game;

void game_init(Game* g);
void game_handle_input(Game* g);
void game_update(Game* g, float dt);
void game_render(Game* g);

#endif
