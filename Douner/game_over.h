#ifndef _GAME_OVER_H
#define _GAME_OVER_H
#include "global.h"
#include "menu.h"
#include "rank.h"

MENU_ITEM game_over_menu[];

void game_over_init(int score);
void game_over_update();
void game_over_draw();

#endif // !_GAME_OVER_H
