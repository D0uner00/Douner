#include "hud.h"

// Main.c에 선언된 폰트 전역 변수를 공유해서 사용합니다.
extern ALLEGRO_FONT* menu_font;

hud_init() {

}

void hud_draw(GameState* game)
{
    // 아이템을 먹어서 올라간 점수(game->score)를 화면 우측 상단에 출력합니다. 
    al_draw_textf(
        menu_font,
        al_map_rgb(255, 255, 255), // 하얀색
        SCREEN_WIDTH - 20, 20,      // 우측 상단 좌표
        ALLEGRO_ALIGN_RIGHT,       // 우측 정렬
        "SCORE: %d",             // 000010 형식
        game->score
    );
}

hud_deinit() {

}