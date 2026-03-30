#include "hud.h"

// Main.c에 선언된 폰트 전역 변수를 공유해서 사용합니다.
extern ALLEGRO_FONT* menu_font;

static ALLEGRO_FONT* hud_font = NULL;

static ALLEGRO_BITMAP* img_start = NULL;
static ALLEGRO_BITMAP* img_next = NULL;
static ALLEGRO_BITMAP* img_final = NULL;

typedef enum { TRANS_NONE, TRANS_START, TRANS_NEXT, TRANS_FINAL } TransType;
static TransType current_trans = TRANS_NONE;
static int trans_timer = 0;
static bool is_start_shown = false;

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

    img_start = al_load_bitmap("START.png");
    img_next = al_load_bitmap("NEXT.png");
    img_final = al_load_bitmap("FINAL.png");
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

void reset_stage_transition() {
    current_trans = TRANS_NONE;
    trans_timer = 0;
    is_start_shown = false;
}

bool update_stage_transition(GameState* game) {
    // 1. 이미 팝업이 떠 있는 상태라면 타이머만 줄입니다.
    if (current_trans != TRANS_NONE) {
        trans_timer--;
        if (trans_timer <= 0) {
            current_trans = TRANS_NONE; // 시간 다 되면 팝업 끄기
        }
        return true; // 게임 멈춤 신호
    }

    // 2. 게임이 처음 시작됐을 때 (START 팝업)
    if (!is_start_shown) {
        is_start_shown = true;
        current_trans = TRANS_START;
        trans_timer = 60; // 60프레임 정지
        return true;
    }

    // 3. 1000점 도달 시 2단계로 가며 NEXT 팝업
    if (game->score >= 100 && game->difficulty == 1) {
        game->difficulty = 2;
        current_trans = TRANS_NEXT;
        trans_timer = 60;
        return true;
    }
    else if (game->score >= 200 && game->difficulty == 2) {
        game->difficulty = 3;
        current_trans = TRANS_FINAL;
        trans_timer = 60;
        return true;
    }


    return false; // 평소에는 게임 계속 진행
}

void draw_stage_transition() {
    ALLEGRO_BITMAP* target_img = NULL;

    // 현재 상태에 따라 그릴 이미지를 선택합니다.
    if (current_trans == TRANS_START) target_img = img_start;
    else if (current_trans == TRANS_NEXT) target_img = img_next;
    else if (current_trans == TRANS_FINAL) target_img = img_final;

    // 선택된 이미지가 있다면 화면 중앙에 그립니다.
    if (target_img) {
        int w = al_get_bitmap_width(target_img);
        int h = al_get_bitmap_height(target_img);
        float draw_x = (SCREEN_WIDTH - w) / 2.0f;
        float draw_y = (SCREEN_HEIGHT - h) / 2.0f;

        al_draw_bitmap(target_img, draw_x, draw_y, 0);
    }
}

void hud_deinit() {
    if (hud_font) al_destroy_font(hud_font);

    if (img_start) al_destroy_bitmap(img_start);
    if (img_next) al_destroy_bitmap(img_next);
    if (img_final) al_destroy_bitmap(img_final);
}

