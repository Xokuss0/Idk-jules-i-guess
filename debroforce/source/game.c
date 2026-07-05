#include "game.h"
#include "hal/hal.h"
#include "assets.h"
#include <stdlib.h>

void game_init(Game* g) {
    g->state = STATE_MENU;
    map_init(&g->map);
    ps_init(&g->ps);
    projectiles_init();
    enemies_init();
    rescue_init();
    bro_init(&g->current_bro, BRO_RAMBRO, 100, 100);
    g->camera_x = 0;
    g->camera_y = 0;
    g->running = true;

    // Initial spawns
    enemies_spawn(300, 100);
    enemies_spawn(500, 100);
    rescue_spawn(200, 100);
}

void game_handle_input(Game* g) {
    if (g->state == STATE_MENU) {
        if (hal_key_pressed(HAL_KEY_A) || hal_key_pressed(HAL_KEY_START)) {
            g->state = STATE_PLAYING;
        }
    } else if (g->state == STATE_PLAYING) {
        g->current_bro.entity.vx = 0;
        if (hal_key_held(HAL_KEY_LEFT)) {
            g->current_bro.entity.vx = -150;
            g->current_bro.entity.facing = -1;
        }
        if (hal_key_held(HAL_KEY_RIGHT)) {
            g->current_bro.entity.vx = 150;
            g->current_bro.entity.facing = 1;
        }
        if (hal_key_pressed(HAL_KEY_UP) && g->current_bro.entity.on_ground) {
            g->current_bro.entity.vy = -350;
        }
        if (hal_key_held(HAL_KEY_A)) {
            bro_attack(&g->current_bro, &g->map);
        }
        if (hal_key_pressed(HAL_KEY_B)) {
            bro_special(&g->current_bro, &g->map);
        }

        if (hal_key_pressed(HAL_KEY_START)) {
            g->state = STATE_MENU;
        }
    }
}

void game_update(Game* g, float dt) {
    if (g->state != STATE_PLAYING) return;

    BroType old_type = g->current_bro.type;
    rescue_update(&g->current_bro.entity, &g->current_bro.type);
    if (g->current_bro.type != old_type) {
         float x = g->current_bro.entity.x;
         float y = g->current_bro.entity.y;
         bro_init(&g->current_bro, g->current_bro.type, x, y);
    }

    bro_update(&g->current_bro, &g->map, dt);
    enemies_update(&g->map, dt, g->current_bro.entity.x);
    projectiles_update(&g->map, dt, &g->ps);
    ps_update(&g->ps, dt);

    // Camera follow
    g->camera_x = (int)g->current_bro.entity.x - TOP_WIDTH / 2;
    g->camera_y = (int)g->current_bro.entity.y - TOP_HEIGHT / 2;
    if (g->camera_x < 0) g->camera_x = 0;
    if (g->camera_y < 0) g->camera_y = 0;
    if (g->camera_x > MAP_WIDTH * TILE_SIZE - TOP_WIDTH) g->camera_x = MAP_WIDTH * TILE_SIZE - TOP_WIDTH;
    if (g->camera_y > MAP_HEIGHT * TILE_SIZE - TOP_HEIGHT) g->camera_y = MAP_HEIGHT * TILE_SIZE - TOP_HEIGHT;
    hal_set_camera(g->camera_x, g->camera_y);
}

void game_render(Game* g) {
    hal_start_frame();

    if (g->state == STATE_MENU) {
        hal_select_screen(true);
        hal_clear_screen(0, 0, 0);
        Texture eagle = assets_get(AT_EAGLE);
        if (eagle) {
            hal_draw_texture(eagle, (TOP_WIDTH - 128) / 2, 40, 128, 128, false);
        }
        hal_draw_rect((TOP_WIDTH - 100) / 2, 180, 100, 30, COLOR_RED);

        hal_select_screen(false);
        hal_clear_screen(10, 10, 10);
    } else {
        // Top Screen Gameplay
        hal_select_screen(true);
        hal_clear_screen(135, 206, 235);
        map_draw(&g->map);
        enemies_draw();
        rescue_draw();
        projectiles_draw();
        ps_draw(&g->ps);

        AssetID aid = AT_RAMBRO;
        if (g->current_bro.type == BRO_BROMINATOR) aid = HAL_AT_BROMINATOR;
        else if (g->current_bro.type == BRO_BLADE) aid = AT_BLADE;
        Texture tex = assets_get(aid);
        if (tex) {
            hal_draw_texture(tex, (int)g->current_bro.entity.x - 10, (int)g->current_bro.entity.y - 16, 32, 32, g->current_bro.entity.facing < 0);
        } else {
            hal_draw_rect((int)g->current_bro.entity.x, (int)g->current_bro.entity.y, 12, 16, COLOR_BLUE);
        }

        // Bottom Screen HUD
        hal_select_screen(false);
        hal_clear_screen(30, 30, 30);
        int mm_offset_x = (BOTTOM_WIDTH - MAP_WIDTH * 2) / 2;
        int mm_offset_y = (BOTTOM_HEIGHT - MAP_HEIGHT * 2) / 2;
        hal_draw_rect(mm_offset_x - 4, mm_offset_y - 4, MAP_WIDTH * 2 + 8, MAP_HEIGHT * 2 + 8, COLOR_BLACK);
        for (int my = 0; my < MAP_HEIGHT; my++) {
            for (int mx = 0; mx < MAP_WIDTH; mx++) {
                if (map_get_tile(&g->map, mx, my) != TILE_EMPTY) {
                    hal_draw_rect(mm_offset_x + mx * 2, mm_offset_y + my * 2, 2, 2, COLOR_WHITE);
                }
            }
        }
        hal_draw_rect(mm_offset_x + ((int)g->current_bro.entity.x / TILE_SIZE) * 2, mm_offset_y + ((int)g->current_bro.entity.y / TILE_SIZE) * 2, 2, 2, COLOR_RED);
    }

    hal_end_frame();
}
