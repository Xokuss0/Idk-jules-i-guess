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

    Texture tex_rambro = hal_load_texture("gfx/rambro.png");
    Texture tex_brominator = hal_load_texture("gfx/brominator.png");
    Texture tex_blade = hal_load_texture("gfx/blade.png");

    enemies_spawn(300, 100);
    enemies_spawn(500, 100);
    rescue_spawn(200, 100);

    float dt = 1.0f / 60.0f;

    while (hal_is_running()) {
        hal_start_frame();

        // Input
        current_bro.entity.vx = 0;
        if (hal_key_held(HAL_KEY_LEFT)) {
            current_bro.entity.vx = -150;
            current_bro.entity.facing = -1;
        }
        if (hal_key_held(HAL_KEY_RIGHT)) {
            current_bro.entity.vx = 150;
            current_bro.entity.facing = 1;
        }
        if (hal_key_pressed(HAL_KEY_UP) && current_bro.entity.on_ground) {
            current_bro.entity.vy = -300;
        }

        if (hal_key_held(HAL_KEY_A)) {
            bro_attack(&current_bro, &game_map);
        }
        if (hal_key_pressed(HAL_KEY_B)) {
            bro_special(&current_bro, &game_map);
        }

        // Update
        BroType old_type = current_bro.type;
        rescue_update(&current_bro.entity, &current_bro.type);
        if (current_bro.type != old_type) {
             float x = current_bro.entity.x;
             float y = current_bro.entity.y;
             bro_init(&current_bro, current_bro.type, x, y);
        }

        bro_update(&current_bro, &game_map, dt);
        enemies_update(&game_map, dt, current_bro.entity.x);
        projectiles_update(&game_map, dt);
        ps_update(&g_ps, dt);

        // Draw Top Screen
        hal_select_screen(true);
        hal_clear_screen(0, 0, 0);

        map_draw(&game_map);
        enemies_draw();
        rescue_draw();
        projectiles_draw();
        ps_draw(&g_ps);

        Texture current_tex = NULL;
        if (current_bro.type == BRO_RAMBRO) current_tex = tex_rambro;
        else if (current_bro.type == BRO_BROMINATOR) current_tex = tex_brominator;
        else if (current_bro.type == BRO_BLADE) current_tex = tex_blade;

        if (current_tex) {
             hal_draw_texture(current_tex, (int)current_bro.entity.x - 10, (int)current_bro.entity.y - 16, 32, 32, current_bro.entity.facing < 0);
        } else {
            Color bro_color = COLOR_BLUE;
            if (current_bro.type == BRO_BROMINATOR) bro_color = 0xFF8C00FF;
            if (current_bro.type == BRO_BLADE) bro_color = 0x800080FF;
            if (current_bro.special_active) bro_color = COLOR_YELLOW;
            hal_draw_rect((int)current_bro.entity.x, (int)current_bro.entity.y, (int)current_bro.entity.w, (int)current_bro.entity.h, bro_color);
        }

        // Draw Bottom Screen
        hal_select_screen(false);
        hal_clear_screen(20, 20, 20);

        for (int my = 0; my < MAP_HEIGHT; my++) {
            for (int mx = 0; mx < MAP_WIDTH; mx++) {
                if (map_get_tile(&game_map, mx, my) != TILE_EMPTY) {
                    hal_draw_rect(mx + 10, my + 10, 1, 1, COLOR_WHITE);
                }
            }
        }
        hal_draw_rect((int)current_bro.entity.x / TILE_SIZE + 10, (int)current_bro.entity.y / TILE_SIZE + 10, 1, 1, COLOR_RED);

        if (hal_key_pressed(HAL_KEY_START)) {
            break;
        }

        hal_end_frame();
    }

    hal_exit();
    return 0;
}
