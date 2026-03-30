#include <allegro5/allegro5.h>
#include "keyboard.h"

unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
    switch (event->type)
    {
    case ALLEGRO_EVENT_KEY_DOWN:
        key[event->keyboard.keycode] = KEY_SEEN | KEY_DOWN;
        break;
    case ALLEGRO_EVENT_KEY_UP:
        key[event->keyboard.keycode] &= ~KEY_DOWN;
        break;
    }
}

bool key_hold(int keycode) {
    if (key[keycode] & KEY_DOWN) {
		key[keycode] &= ~KEY_SEEN;

        return true;
    }
    return false;
}

bool key_pressed(int keycode) {
    if ((key[keycode] & KEY_DOWN) && (key[keycode] & KEY_SEEN)) {
        key[keycode] &= ~KEY_SEEN;  // ?? ?? ?????? ????
        return true;
    }
    return false;
}

