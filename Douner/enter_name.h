#ifndef ENTER_NAME_H
#define ENTER_NAME_H
#include "global.h"

#define NAME_MAX 16

typedef struct {
    char buffer[NAME_MAX];
    int length;
    Rect box;
} NameInput;

//void name_input_init(NameInput* name);
//void name_input_update(NameInput* name, ALLEGRO_EVENT* event);
//void name_input_draw(NameInput* name);

#endif