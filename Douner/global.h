
#ifndef _GLOBAL_H_ 
#define _GLOBAL_H_
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

#define NAME_MAX 16

typedef struct {
    int score;
    int life;
    float hp;
    int difficulty;
    char player_name[NAME_MAX]; // 이 플레이어의 주인 이름
} GameState;

typedef enum {
    SCREEN_MENU,
    SCREEN_PLAY,
    SCREEN_NAME_INPUT
} GameScreen;

typedef struct {
    int x, y, w, h;
} Rect;

void must_init(bool test, const char* description);
void game_init(GameState* game);


//		임시			//
void draw_map();

#endif
