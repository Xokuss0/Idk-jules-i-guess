#include <stddef.h>
#include "enemy.h"
#include "hal/hal.h"
#include "bro.h"
#include <stdlib.h>

Enemy g_enemies[MAX_ENEMIES];
static Texture tex_mook = NULL;

void enemies_init(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) g_enemies[i].active = false;
    tex_mook = hal_load_texture("gfx/mook.png");
}

void enemies_spawn(float x, float y) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!g_enemies[i].active) {
            g_enemies[i].active = true;
            g_enemies[i].entity.x = x;
            g_enemies[i].entity.y = y;
            g_enemies[i].entity.vx = 50.0f;
            g_enemies[i].entity.vy = 0;
            g_enemies[i].entity.w = 12;
            g_enemies[i].entity.h = 16;
            g_enemies[i].entity.facing = 1;
            g_enemies[i].attack_timer = 1.0f;
            return;
        }
    }
}

void enemies_update(Map* map, float dt, float player_x) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!g_enemies[i].active) continue;

        if (entity_check_collision(&g_enemies[i].entity, map, g_enemies[i].entity.vx * dt, 0)) {
            g_enemies[i].entity.vx *= -1;
            g_enemies[i].entity.facing *= -1;
        }

        entity_update(&g_enemies[i].entity, map, dt);

        g_enemies[i].attack_timer -= dt;
        if (g_enemies[i].attack_timer <= 0) {
             if (abs(g_enemies[i].entity.x - player_x) < 200) {
                 projectiles_spawn(g_enemies[i].entity.x, g_enemies[i].entity.y + 8, g_enemies[i].entity.facing * 200.0f, 0, false);
                 g_enemies[i].attack_timer = 2.0f;
             }
        }
    }
}

void enemies_draw(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (g_enemies[i].active) {
            if (tex_mook) {
                hal_draw_texture(tex_mook, (int)g_enemies[i].entity.x - 10, (int)g_enemies[i].entity.y - 16, 32, 32, g_enemies[i].entity.facing < 0);
            } else {
                hal_draw_rect((int)g_enemies[i].entity.x, (int)g_enemies[i].entity.y, 12, 16, COLOR_RED);
            }
        }
    }
}
