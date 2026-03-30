#include "Background.h"

void init_background(Background* bg) {
    bg->image = al_load_bitmap("background\\background_1.png");
    if (!bg->image) {
        fprintf(stderr, "배경 이미지 로드 실패!\n");
        return;
    }
    bg->x = 0;
    bg->speed = 3.0f;
    bg->width = al_get_bitmap_width(bg->image); // 1024
}

void update_background(Background* bg) {
    // 배경을 왼쪽으로 이동
    bg->x -= bg->speed;

    // 이미지가 화면을 완전히 빠져나가면(width만큼 이동하면) 다시 0으로 리셋
    if (bg->x <= -bg->width) {
        bg->x = 0;
    }
}

void draw_background(Background* bg) {
    if (!bg->image) return;

    // 첫 번째 배경 그리기
    al_draw_bitmap(bg->image, bg->x, 0, 0);
    // 빈 공간을 채울 두 번째 배경 그리기 (첫 번째 배경 바로 뒤에 붙임)
    al_draw_bitmap(bg->image, bg->x + bg->width, 0, 0);
}

void destroy_background(Background* bg) {
    if (bg->image) {
        al_destroy_bitmap(bg->image);
    }
}
