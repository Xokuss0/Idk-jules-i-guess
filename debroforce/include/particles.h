#ifndef PARTICLES_H
#define PARTICLES_H

#include "hal/hal.h"

#define MAX_PARTICLES 128

typedef struct {
    float x, y;
    float vx, vy;
    float life;
    Color color;
    bool active;
} Particle;

typedef struct {
    Particle particles[MAX_PARTICLES];
} ParticleSystem;

void ps_init(ParticleSystem* ps);
void ps_spawn(ParticleSystem* ps, float x, float y, Color color);
void ps_update(ParticleSystem* ps, float dt);
void ps_draw(ParticleSystem* ps);

#endif
