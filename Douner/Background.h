#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "global.h"

typedef struct {
    ALLEGRO_BITMAP* image;
    float x;
    float speed;
    int width;
} Background;

void init_background(Background* bg);
void update_background(Background* bg);
void draw_background(Background* bg);
void destroy_background(Background* bg);

#endif