#ifndef RESCUE_H
#define RESCUE_H

#include "bro.h"
#include "entity.h"

#define MAX_CAGES 8

typedef struct {
    float x, y;
    bool active;
} Cage;

void rescue_init(void);
void rescue_spawn(float x, float y);
void rescue_update(Entity* player, BroType* current_bro_type);
void rescue_draw(void);

#endif
