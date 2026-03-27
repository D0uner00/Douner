#ifndef  _MENU_H_
#define _MENU_H_

#include "global.h"
#include "keyboard.h"
#include "mouse.h"

enum MenuState {
	MENU_CONTINUE = 0,
	MENU_BACK,
	MENU_EXIT,
};

enum MenuMessage {
	MSG_INIT =0,
	MSG_DRAW,
	MSG_INPUT,
	MSG_WIDTH,
	MSG_HEIGHT,
	MSG_TICK
};

#define FLAG_NONE		0
#define FLAG_SELECTABLE 1
#define FLAG_SELECTED	2

typedef struct MENU_ITEM MENU_ITEM;

struct MENU_ITEM {
	int (*handler)(MENU_ITEM* item, int msg, int param);
	const char* text;
	int flags;
	int value;
	void* data;
	void (*on_click)(void);
	int x, y, w, h;
};

/*#define MENU_TEXT(text) \
	{ menu_text_handler, text , FLAG_NONE, 0, NULL, NULL, 0, 0 ,0 ,0 }*/

#define MENU_BUTTON(text, callback) \
	{ menu_button_handler, text , FLAG_SELECTABLE, 0, NULL, callback, 0, 0 ,0 ,0 }

#define MENU_SPACE(height) \
	{ menu_space_handler, "" , FLAG_NONE, height, NULL, NULL, 0, 0 ,0 ,0 }

#define MENU_END() \
	{ NULL, NULL, 0, 0, NULL, NULL, 0, 0 ,0 ,0 }

void menu_init(MENU_ITEM *menu);
int menu_update(MENU_ITEM* menu);
void menu_draw(MENU_ITEM* menu);

//int menu_text_handler(MENU_ITEM* item, int msg, int param);
int menu_space_handler(MENU_ITEM* item, int msg, int param);
int menu_button_handler(MENU_ITEM* item, int msg, int param);

#endif // ! _MENU_H_
