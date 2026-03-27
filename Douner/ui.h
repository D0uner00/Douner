#ifndef UI_H
#define UI_H
#include "global.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

Rect make_center_rect(int w, int h);
void draw_box(Rect r);
void draw_center_text(const char* text, int y, ALLEGRO_COLOR color);

#endif