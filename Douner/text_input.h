#ifndef ENTER_NAME_H
#define ENTER_NAME_H
#include "global.h"

typedef struct {
    char buffer[NAME_MAX];
    int length;
    int cursor_visible;
    Rect box;
    int done;
} TextInput;

void text_input_init(TextInput* name);
void text_input_update(TextInput* name, ALLEGRO_EVENT* event);
void text_input_draw(TextInput* name);

#endif