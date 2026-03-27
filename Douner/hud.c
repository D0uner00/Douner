#include "hud.h"

// Main.c에 선언된 폰트 전역 변수를 공유해서 사용합니다.
extern ALLEGRO_FONT* menu_font;

static ALLEGRO_FONT* hud_font = NULL;

void hud_init()
{
    al_init_ttf_addon();

    // 폰트 파일명, 크기(32), 플래그(0)
    // "arial.ttf"는 프로젝트 폴더에 실제로 있어야 합니다.
    hud_font = al_load_ttf_font("RiaSans-Bold.ttf", 20, 0);

    // 만약 폰트 파일 로드에 실패하면 기본 폰트로 대체합니다.
    if (!hud_font) {
        hud_font = al_create_builtin_font();
    }
}

void hud_update(GameState* game) {
    // 1. 초당 조금씩 줄어드는 HP (30fps 기준 프레임당 0.1씩 감소 시 초당 3 감소)
    if (game->hp > 0) {
        game->hp -= 0.02f;
    }
}

void hud_draw(GameState* game)
{
    // 상단바 배경 (640x50)
    al_draw_filled_rectangle(0, 0, 640, 50, al_map_rgba(0, 0, 0, 100));
    al_draw_line(0, 50, 640, 50, al_map_rgb(255, 255, 255), 2);

    // --- HP 섹션 ---
    al_draw_text(menu_font, al_map_rgb(255, 255, 255), 20, 15, 0, "HP");

    // HP 바 테두리
    al_draw_rectangle(60, 15, 260, 35, al_map_rgb(255, 255, 255), 2);

    // 실제 HP 채우기 (현재 HP 비율 계산)
    float hp_width = (game->hp / 100.0f) * 196.0f; // 최대폭 200에서 테두리 제외 약 196
    if (hp_width < 0) hp_width = 0;

    al_draw_filled_rectangle(62, 17, 62 + hp_width, 33, al_map_rgb(255, 50, 50));

    // 아이템을 먹어서 올라간 점수(game->score)를 화면 우측 상단에 출력합니다. 
    al_draw_textf(
        hud_font,
        al_map_rgb(255, 255, 255), // 하얀색
        SCREEN_WIDTH - 20, 10,      // 우측 상단 좌표
        ALLEGRO_ALIGN_RIGHT,       // 우측 정렬
        "SCORE: %d",             // 000010 형식
        game->score
    );
}

void hud_deinit() {
    if (hud_font) {
        al_destroy_font(hud_font);
        hud_font = NULL;
    }
}