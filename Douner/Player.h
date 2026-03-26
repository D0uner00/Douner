#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "global.h"

// 달리기 자르기 정보
#define RUN_CROP_X 43 
#define RUN_CROP_Y 33
#define RUN_SRC_W 45
#define RUN_SRC_H 45
#define RUN_DEST_W 105
#define RUN_DEST_H 130 

//점프 자르기 정보 
#define JUMP_CROP_X 32
#define JUMP_CROP_Y 10
#define JUMP_SRC_W 64
#define JUMP_SRC_H 90
#define JUMP_DEST_W 140
#define JUMP_DEST_H 230

//프레임 수
#define MAX_RUN_FRAMES 10
#define MAX_JUMP_FRAMES 6

//플레이어 시작 위치
#define StartX 100
#define StartY SCREEN_HEIGHT - GROUND_HEIGHT - RUN_DEST_H

typedef struct player {
	int runFrame;
	int jumpFrame;
	int jumpDirection;
	float baseY;
	float jumpSpeed;
	float maxJumpHeight;
	float x, y;
	bool isJumping;

	ALLEGRO_BITMAP* runSheet;
	ALLEGRO_BITMAP* jumpSheet;
}Player;

void init_player(Player* p);
void update_player(Player* p);
void draw_player(Player* p);
void destroy_player(Player* p);

//		임시			//
void draw_map();
#endif // !_PLAYER_H_
