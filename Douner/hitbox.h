#ifndef HITBOX_H
#define HITBOX_H
#include "global.h"

typedef struct {
    int offset_x;
    int offset_y;
    int w;
    int h;
} Hitbox;

int collide_rect(Rect a, Rect b);

#endif