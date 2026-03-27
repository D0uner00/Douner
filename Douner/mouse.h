#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "global.h"

bool mouse_button_down(int button);
bool mouse_button_pressed(int button);

int mouse_x();
int mouse_y();

void mouse_update(ALLEGRO_EVENT* event);
void mouse_tick();
#endif // !_MOUSE_H_
