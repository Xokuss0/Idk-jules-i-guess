#include "rescue.h"
#include "hal/hal.h"
#include "bro.h"
#include <stdlib.h>
#include <stddef.h>

Cage g_cages[MAX_CAGES];
static Texture tex_cage = NULL;

void rescue_init(void) {
    for (int i = 0; i < MAX_CAGES; i++) g_cages[i].active = false;
    tex_cage = hal_load_texture("gfx/cage.png");
}

void rescue_spawn(float x, float y) {
    for (int i = 0; i < MAX_CAGES; i++) {
        if (!g_cages[i].active) {
            g_cages[i].active = true;
            g_cages[i].x = x;
            g_cages[i].y = y;
            return;
        }
    }
}

void rescue_update(Entity* player, BroType* current_bro_type) {
    for (int i = 0; i < MAX_CAGES; i++) {
        if (!g_cages[i].active) continue;

        if (player->x < g_cages[i].x + 16 && player->x + player->w > g_cages[i].x &&
            player->y < g_cages[i].y + 16 && player->y + player->h > g_cages[i].y) {

            g_cages[i].active = false;
            BroType next = rand() % 3;
            *current_bro_type = next;
        }
    }
}

void rescue_draw(void) {
    for (int i = 0; i < MAX_CAGES; i++) {
        if (g_cages[i].active) {
            if (tex_cage) {
                hal_draw_texture(tex_cage, (int)g_cages[i].x, (int)g_cages[i].y, 16, 16, false);
            } else {
                hal_draw_rect((int)g_cages[i].x, (int)g_cages[i].y, 16, 16, COLOR_YELLOW);
                hal_draw_rect((int)g_cages[i].x + 4, (int)g_cages[i].y + 4, 8, 8, COLOR_WHITE);
            }
        }
    }
}
