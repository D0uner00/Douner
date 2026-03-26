#include "global.h"

void must_init(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}


//      임시 배경      //
void draw_map() {
    al_clear_to_color(al_map_rgb(135, 206, 235));
    al_draw_filled_rectangle(0, SCREEN_HEIGHT - GROUND_HEIGHT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        al_map_rgb(34, 139, 34));
}