#include "game_over.h"

extern void on_back_to_menu();
Rect box;
ALLEGRO_FONT* menu_font;

static char score_text[64];
MENU_ITEM game_over_menu[] = {
	MENU_TEXT("GAME OVER"),
	MENU_SPACE(10),
	MENU_TEXT(""),
	MENU_BUTTON("Back to Menu", on_back_to_menu),
	MENU_END()
};

void game_over_init(int score) {
	sprintf(score_text, "Your Score: %d", score);
	game_over_menu[2].text = score_text;
	menu_init(game_over_menu);
}

void game_over_update() {
	menu_update(game_over_menu);
}

void game_over_draw() {
	draw_box(box);
	for (int i = 0; game_over_menu[i].handler != NULL; ++i) {

        // "GAME OVER"는 빨간색으로, 나머지는 흰색으로 그리기
        ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
        if (i == 0) color = al_map_rgb(255, 0, 0);

        al_draw_text(menu_font, color,
            game_over_menu[i].x,
            game_over_menu[i].y,
            ALLEGRO_ALIGN_CENTER,
            game_over_menu[i].text);
    }
}