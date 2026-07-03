#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hal/hal.h"
#include "map.h"
#include "particles.h"
#include "bro.h"
#include "enemy.h"
#include "rescue.h"

ParticleSystem g_ps;

int main(int argc, char* argv[]) {
    srand(time(NULL));
    hal_init();

    Map game_map;
    map_init(&game_map);
    ps_init(&g_ps);
    projectiles_init();
    enemies_init();
    rescue_init();

    Bro current_bro;
    bro_init(&current_bro, BRO_RAMBRO, 100, 100);

    // Initial spawns
    enemies_spawn(300, 100);
    enemies_spawn(500, 100);
    rescue_spawn(200, 100);

    float dt = 1.0f / 60.0f;

    while (hal_is_running()) {
        hal_start_frame();

        // Input
        current_bro.entity.vx = 0;
        if (hal_key_held(KEY_LEFT)) {
            current_bro.entity.vx = -150;
            current_bro.entity.facing = -1;
        }
        if (hal_key_held(KEY_RIGHT)) {
            current_bro.entity.vx = 150;
            current_bro.entity.facing = 1;
        }
        if (hal_key_pressed(KEY_UP) && current_bro.entity.on_ground) {
            current_bro.entity.vy = -300;
        }

        if (hal_key_held(KEY_A)) {
            bro_attack(&current_bro, &game_map);
        }
        if (hal_key_pressed(KEY_B)) {
            bro_special(&current_bro, &game_map);
        }

        // Update
        BroType old_type = current_bro.type;
        rescue_update(&current_bro.entity, &current_bro.type);
        if (current_bro.type != old_type) {
             // Swap keeping position
             float x = current_bro.entity.x;
             float y = current_bro.entity.y;
             bro_init(&current_bro, current_bro.type, x, y);
        }

        bro_update(&current_bro, &game_map, dt);
        enemies_update(&game_map, dt, current_bro.entity.x);
        projectiles_update(&game_map, dt);
        ps_update(&g_ps, dt);

        // Draw Top Screen
        map_draw(&game_map);
        enemies_draw();
        rescue_draw();
        projectiles_draw();
        ps_draw(&g_ps);

        // Draw Player
        Color bro_color = COLOR_BLUE;
        if (current_bro.type == BRO_BROMINATOR) bro_color = 0xFF8C00FF; // Orange
        if (current_bro.type == BRO_BLADE) bro_color = 0x800080FF; // Purple
        if (current_bro.special_active) bro_color = COLOR_YELLOW;
        hal_draw_rect((int)current_bro.entity.x, (int)current_bro.entity.y, (int)current_bro.entity.w, (int)current_bro.entity.h, bro_color);

        // Draw Bottom Screen (Simulated on PC as bottom half)
        // Mini-map
        for (int my = 0; my < MAP_HEIGHT; my++) {
            for (int mx = 0; mx < MAP_WIDTH; mx++) {
                if (map_get_tile(&game_map, mx, my) != TILE_EMPTY) {
                    hal_draw_rect(mx + 10, my + TOP_HEIGHT + 10, 1, 1, COLOR_WHITE);
                }
            }
        }
        // Player on mini-map
        hal_draw_rect((int)current_bro.entity.x / TILE_SIZE + 10, (int)current_bro.entity.y / TILE_SIZE + TOP_HEIGHT + 10, 1, 1, COLOR_RED);

        // UI Text (Stub)
        hal_draw_rect(100, TOP_HEIGHT + 50, 100, 10, COLOR_GREEN); // Health bar stub

        if (hal_key_pressed(KEY_START)) {
            break;
        }

        hal_end_frame();
    }

    hal_exit();
    return 0;
}
