#ifndef ENTER_NAME_H
#define ENTER_NAME_H
#include "global.h"

typedef struct {
    char buffer[NAME_MAX];
    int length;
    int cursor_visible;
    Rect box;
    int done;
} NameInput;

void name_input_init(NameInput* name);
void name_input_update(NameInput* name, ALLEGRO_EVENT* event);
void name_input_draw(NameInput* name);

#endif