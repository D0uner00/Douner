#include "global.h"

void must_init(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void draw_map() {
    al_clear_to_color(al_map_rgb(135, 206, 235));
    al_draw_filled_rectangle(0, SCREEN_HEIGHT - GROUND_HEIGHT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        al_map_rgb(34, 139, 34));
}

/*
int collide(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
{
    return !(ax > bx + bw || ax + aw < bx ||
        ay > by + bh || ay + ah < by);
}*/

int collide_rect(Rect a, Rect b)
{
    return !(a.x > b.x + b.w ||
        a.x + a.w < b.x ||
        a.y > b.y + b.h ||
        a.y + a.h < b.y);
}

void game_init(GameState* game)
{
    game->score = 0;
    game->life = 3;
    game->difficulty = 0;
}
