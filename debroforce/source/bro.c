#include <stdlib.h>
#include "bro.h"
#include "hal/hal.h"
#include <math.h>

Projectile g_projectiles[MAX_PROJECTILES];

void projectiles_init(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) g_projectiles[i].active = false;
}

void projectiles_spawn(float x, float y, float vx, float vy, bool is_grenade) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!g_projectiles[i].active) {
            g_projectiles[i].active = true;
            g_projectiles[i].x = x;
            g_projectiles[i].y = y;
            g_projectiles[i].vx = vx;
            g_projectiles[i].vy = vy;
            g_projectiles[i].is_grenade = is_grenade;
            return;
        }
    }
}

void projectiles_update(Map* map, float dt, void* ps) {
    ParticleSystem* particles = (ParticleSystem*)ps;
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!g_projectiles[i].active) continue;
        g_projectiles[i].x += g_projectiles[i].vx * dt;
        g_projectiles[i].y += g_projectiles[i].vy * dt;
        if (g_projectiles[i].is_grenade) g_projectiles[i].vy += 600.0f * dt;
        int tx = g_projectiles[i].x / TILE_SIZE;
        int ty = g_projectiles[i].y / TILE_SIZE;
        if (map_get_tile(map, tx, ty) != TILE_EMPTY) {
            if (g_projectiles[i].is_grenade) {
                map_explode(map, g_projectiles[i].x, g_projectiles[i].y, 48, particles);
            } else {
                map_set_tile(map, tx, ty, TILE_EMPTY);
                if (particles) ps_spawn(particles, g_projectiles[i].x, g_projectiles[i].y, COLOR_YELLOW);
            }
            g_projectiles[i].active = false;
        }
        if (g_projectiles[i].x < 0 || g_projectiles[i].x > MAP_WIDTH * TILE_SIZE ||
            g_projectiles[i].y < 0 || g_projectiles[i].y > MAP_HEIGHT * TILE_SIZE) {
            g_projectiles[i].active = false;
        }
    }
}

void projectiles_draw(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!g_projectiles[i].active) continue;
        Color c = g_projectiles[i].is_grenade ? COLOR_GREEN : COLOR_YELLOW;
        hal_draw_rect((int)g_projectiles[i].x - 1, (int)g_projectiles[i].y - 1, 3, 3, c);
    }
}

void bro_init(Bro* b, BroType type, float x, float y) {
    b->entity.x = x;
    b->entity.y = y;
    b->entity.vx = 0;
    b->entity.vy = 0;
    b->entity.w = 12;
    b->entity.h = 16;
    b->entity.on_ground = false;
    b->entity.facing = 1;
    b->type = type;
    b->attack_cooldown = 0;
    b->special_cooldown = 0;
    b->special_active = false;
    b->special_timer = 0;
}

void bro_update(Bro* b, Map* map, float dt) {
    if (b->attack_cooldown > 0) b->attack_cooldown -= dt;
    if (b->special_cooldown > 0) b->special_cooldown -= dt;
    if (b->special_active) {
        b->special_timer -= dt;
        if (b->special_timer <= 0) b->special_active = false;
    }
    entity_update(&b->entity, map, dt);
}

void bro_attack(Bro* b, Map* map) {
    if (b->attack_cooldown > 0) return;
    switch (b->type) {
        case BRO_RAMBRO:
            projectiles_spawn(b->entity.x + 6, b->entity.y + 8, b->entity.facing * 400.0f, 0, false);
            b->attack_cooldown = 0.15f;
            break;
        case BRO_BROMINATOR:
            projectiles_spawn(b->entity.x + 6, b->entity.y + 8, b->entity.facing * 500.0f, ((float)rand()/RAND_MAX - 0.5f) * 50.0f, false);
            b->entity.vx -= b->entity.facing * 100.0f;
            b->attack_cooldown = 0.05f;
            break;
        case BRO_BLADE:
            map_explode(map, b->entity.x + b->entity.facing * 20, b->entity.y + 8, 16, NULL);
            b->entity.vx += b->entity.facing * 200.0f;
            b->attack_cooldown = 0.3f;
            break;
    }
}

void bro_special(Bro* b, Map* map) {
    if (b->special_cooldown > 0) return;
    switch (b->type) {
        case BRO_RAMBRO:
            projectiles_spawn(b->entity.x + 6, b->entity.y + 8, b->entity.facing * 250.0f, -200.0f, true);
            b->special_cooldown = 1.0f;
            break;
        case BRO_BROMINATOR:
            b->special_active = true;
            b->special_timer = 2.0f;
            b->special_cooldown = 5.0f;
            break;
        case BRO_BLADE:
            for(int i=-1; i<=1; i++)
                projectiles_spawn(b->entity.x + 6, b->entity.y + 8, b->entity.facing * 400.0f, i * 50.0f, false);
            b->special_cooldown = 0.5f;
            break;
    }
}
