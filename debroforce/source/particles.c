#include "particles.h"
#include <stdlib.h>

void ps_init(ParticleSystem* ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) ps->particles[i].active = false;
}

void ps_spawn(ParticleSystem* ps, float x, float y, Color color) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!ps->particles[i].active) {
            ps->particles[i].active = true;
            ps->particles[i].x = x;
            ps->particles[i].y = y;
            ps->particles[i].vx = ((float)rand() / RAND_MAX - 0.5f) * 100.0f;
            ps->particles[i].vy = ((float)rand() / RAND_MAX - 1.0f) * 150.0f;
            ps->particles[i].life = 1.0f;
            ps->particles[i].color = color;
            return;
        }
    }
}

void ps_update(ParticleSystem* ps, float dt) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (ps->particles[i].active) {
            ps->particles[i].x += ps->particles[i].vx * dt;
            ps->particles[i].y += ps->particles[i].vy * dt;
            ps->particles[i].vy += 400.0f * dt; // Gravity
            ps->particles[i].life -= dt;
            if (ps->particles[i].life <= 0) ps->particles[i].active = false;
        }
    }
}

void ps_draw(ParticleSystem* ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (ps->particles[i].active) {
            hal_draw_rect((int)ps->particles[i].x, (int)ps->particles[i].y, 2, 2, ps->particles[i].color);
        }
    }
}
