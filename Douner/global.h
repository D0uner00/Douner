
#ifndef _GLOBAL_H_ 
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>

#include <allegro5/allegro_primitives.h> // 도형 그리기를 위해 필요
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400
#define GROUND_HEIGHT 70

#define FPS 60.0f
#define GROUND_Y 350.0f
#define GRAVITY 0.5f
#define MAX_OBS 99

typedef struct {
    int score;
    float life;
    int hp;
    int difficulty;
} GameState;

void must_init(bool test, const char* description);
void game_init(GameState* game);


//		임시			//
void draw_map();

#endif
