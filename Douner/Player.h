#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "global.h"

/*
// 달리기 자르기 정보
#define RUN_CROP_X 43 
#define RUN_CROP_Y 33
#define RUN_SRC_W 45
#define RUN_SRC_H 45
#define RUN_DEST_W 120
#define RUN_DEST_H 120

//점프 자르기 정보 
#define JUMP_CROP_X 30
#define JUMP_CROP_Y 10
#define JUMP_SRC_W 60
#define JUMP_SRC_H 70
#define JUMP_DEST_W 150
#define JUMP_DEST_H 230
*/

// 달리기 자르기 정보
#define RUN_CROP_X 40 
#define RUN_CROP_Y 20
#define RUN_SRC_W 60
#define RUN_SRC_H 95
#define RUN_DEST_W 90
#define RUN_DEST_H 140

//점프 자르기 정보 
#define JUMP_CROP_X 40
#define JUMP_CROP_Y 10
#define JUMP_SRC_W 50
#define JUMP_SRC_H 95
#define JUMP_DEST_W 75
#define JUMP_DEST_H 140

//프레임 수
#define MAX_RUN_FRAMES 9
#define MAX_JUMP_FRAMES 6

//플레이어 시작 위치
#define StartX 100
#define StartY 195 //SCREEN_HEIGHT - GROUND_HEIGHT - RUN_DEST_H

typedef enum {
	PLAYER_RUN,
	PLAYER_JUMP,
	PLAYER_SLIDING,
	PLAYER_DEATH
}PlayerState;

typedef struct player {

	float x, y;
	float baseY;

	PlayerState state;

	int runFrame;
	int jumpFrame;

	int jumpDirection;
	float jumpSpeed;
	float maxJumpHeight;

	int hurtTimer;

	ALLEGRO_BITMAP* runSheet;
	ALLEGRO_BITMAP* jumpSheet;

}Player;

void init_player(Player* p);
void update_player(Player* p);
void draw_player(Player* p);
void destroy_player(Player* p);


#endif // !_PLAYER_H_
