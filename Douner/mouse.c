#include "mouse.h"

#define MOUSE_BUTTON 10

static char mouse_array[MOUSE_BUTTON];
static int mx, my;

bool mouse_button_down(int button) {
	return mouse_array[button] & 1;
}

bool mouse_button_pressed(int button) {
	return mouse_array[button] & 2;
}

int mouse_x() {
	return mx;
}

int mouse_y() {
	return my;
}

void mouse_update(ALLEGRO_EVENT* event) {
	switch (event->type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		mouse_array[event->mouse.button] |= 1;
		mouse_array[event->mouse.button] |= 2;
		break;

	case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		mouse_array[event->mouse.button] &= ~1;
		mouse_array[event->mouse.button] |= 4;
		break;

	case ALLEGRO_EVENT_MOUSE_AXES:
		mx = event->mouse.x;
		my = event->mouse.y;
		break;
	}
}

void mouse_tick() {
	for (int i = 0;i < MOUSE_BUTTON;++i) {
		mouse_array[i] &= ~2;
		mouse_array[i] &= ~4;
	}
}