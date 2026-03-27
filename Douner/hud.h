#ifndef HUD_H
#define HUD_H

#include "global.h"

void hud_init();
void hud_update(GameState* game);
void hud_draw(GameState* game);
void hud_deinit();

#endif // !_HUD_H
