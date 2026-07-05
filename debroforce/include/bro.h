#ifndef BRO_H
#define BRO_H

#include "entity.h"
#include "particles.h"

typedef enum {
    BRO_RAMBRO,
    BRO_BROMINATOR,
    BRO_BLADE
} BroType;

typedef struct {
    Entity entity;
    BroType type;
    float attack_cooldown;
    float special_cooldown;
    bool special_active;
    float special_timer;
} Bro;

void bro_init(Bro* b, BroType type, float x, float y);
void bro_update(Bro* b, Map* map, float dt);
void bro_attack(Bro* b, Map* map);
void bro_special(Bro* b, Map* map);

// Projectile system
#define MAX_PROJECTILES 64
typedef struct {
    float x, y, vx, vy;
    bool active;
    bool is_grenade;
} Projectile;

void projectiles_init(void);
void projectiles_spawn(float x, float y, float vx, float vy, bool is_grenade);
void projectiles_update(Map* map, float dt, void* ps);
void projectiles_draw(void);

#endif
