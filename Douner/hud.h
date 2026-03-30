#ifndef HUD_H
#define HUD_H

#include "global.h"

void hud_init();
void hp_update(GameState* game);
void hud_draw(GameState* game);
void hud_deinit();

//stage
void reset_stage_transition();
bool update_stage_transition(GameState* game);
void draw_stage_transition();

#endif // !_HUD_H

