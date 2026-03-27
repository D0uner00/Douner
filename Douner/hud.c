#include "hud.h"

// Main.c에 선언된 폰트 전역 변수를 공유해서 사용합니다.
extern ALLEGRO_FONT* menu_font;

static ALLEGRO_FONT* hud_font = NULL;

void hud_init()
{
    al_init_ttf_addon();

    // 폰트 파일명, 크기(32), 플래그(0)
    // "arial.ttf"는 프로젝트 폴더에 실제로 있어야 합니다.
    hud_font = al_load_ttf_font("RiaSans-Bold.ttf", 15, 0);

    // 만약 폰트 파일 로드에 실패하면 기본 폰트로 대체합니다.
    if (!hud_font) {
        hud_font = al_create_builtin_font();
    }
}

void hud_draw(GameState* game)
{
    // 상단바 배경 및 하단 라인 (기존 유지)
    al_draw_filled_rectangle(0, 0, 640, 50, al_map_rgba(0, 0, 0, 100));
    al_draw_line(0, 50, 640, 50, al_map_rgb(255, 255, 255), 2);

    al_draw_textf(
        hud_font,
        al_map_rgb(255, 255, 255),
        20, 12, 0, "HP");

    // HP 바 테두리 확장: 가로 400px(60~460), 세로 30px(10~40)
    al_draw_rectangle(60, 10, 460, 40, al_map_rgb(255, 255, 255), 2);

    // 실제 HP 채우기 계산: 최대폭 400에서 테두리 두께(좌우 2px씩) 제외 약 396px
    float hp_width = (game->hp / 100.0f) * 396.0f;
    if (hp_width < 0) hp_width = 0;

    // 내부 빨간색 바 (테두리 두께 2px 안쪽으로 배치)
    al_draw_filled_rectangle(62, 12, 62 + hp_width, 38, al_map_rgb(255, 50, 50));

    // SCORE 출력 (기존 유지)
    al_draw_textf(
        hud_font,
        al_map_rgb(255, 255, 255),
        SCREEN_WIDTH - 20, 15,
        ALLEGRO_ALIGN_RIGHT,
        "SCORE: %d",
        game->score
    );
}

void hud_deinit() {
    if (hud_font) {
        al_destroy_font(hud_font);
        hud_font = NULL;
    }
}