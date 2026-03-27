#include "ui.h"
#include "global.h"

extern ALLEGRO_FONT* menu_font;

// 위치 계산
Rect make_center_rect(int w, int h)
{
    Rect r;
    r.w = w;
    r.h = h;
    r.x = (SCREEN_WIDTH / 2) - (w / 2);
    r.y = (SCREEN_HEIGHT / 2) - (h / 2);
    return r;
}

// 그리기
void draw_box(Rect r)
{
    al_draw_filled_rectangle(r.x, r.y, r.x + r.w, r.y + r.h, al_map_rgb(0, 0, 0));
    al_draw_rectangle(r.x, r.y, r.x + r.w, r.y + r.h, al_map_rgb(255, 255, 255), 2);
}

void draw_center_text(const char* text, int y, ALLEGRO_COLOR color)
{
    int center_x = SCREEN_WIDTH / 2;
    al_draw_text(menu_font, color, center_x, y, ALLEGRO_ALIGN_CENTER, text);
}