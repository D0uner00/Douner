#include "Background.h"

void init_background(Background* bg) {
    bg->images[0] = al_load_bitmap("background\\background_0.png");
    bg->images[1] = al_load_bitmap("background\\background_1.png");
    bg->images[2] = al_load_bitmap("background\\background_2.png");
    bg->images[3] = al_load_bitmap("background\\background_3.png");

    for (int i = 0; i < 4; i++) {
        if (!bg->images[i]) {
            fprintf(stderr, "%d번 배경 없음!\n", i + 1);
        }
    }
    bg->current_image = bg->images[0];
    bg->x = 0;
    bg->speed = 3.0f;

    if (bg->current_image) {
        bg->width = al_get_bitmap_width(bg->current_image);
    }
    else {
        bg->width = 1024; //임시 배경 크기
    }
}

void update_background(Background* bg, int difficulty) {
    int index = difficulty;

    if (bg->current_image != bg->images[index]) {
        bg->current_image = bg->images[index];
        bg->x = 0;
        if (bg->current_image) {
            bg->width = al_get_bitmap_width(bg->current_image);
        }
    }

    if (index == 0) {
        bg->x = 0; 
    }
    else {
        bg->x -= bg->speed; 

        if (bg->x <= -bg->width) {
            bg->x = 0;
        }
    }
}

void draw_background(Background* bg) {
    if (!bg->current_image) return;

    al_draw_bitmap(bg->current_image, bg->x, 0, 0);

    if (bg->x < 0) {
        al_draw_bitmap(bg->current_image, bg->x + bg->width, 0, 0);
    }
}

void destroy_background(Background* bg) {
    for (int i = 0; i < 4; i++) {
        if (bg->images[i]) {
            al_destroy_bitmap(bg->images[i]);
        }
    }
}
