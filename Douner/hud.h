#ifndef HUD_H
#define HUD_H

#include "global.h"

void hud_init();
void hp_update(GameState* game);
void hud_draw(GameState* game);
void hud_deinit();

typedef enum { 
	POPUP_NONE, 
	POPUP_START, 
	POPUP_NEXT, 
	POPUP_FINAL 
} PopupType;

//stage
void hud_reset_popup();
void hud_trigger_popup(PopupType type);
bool hud_update_popup();
void hud_draw_popup();

#endif // !_HUD_H

