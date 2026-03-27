#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "global.h"

/*
// �޸��� �ڸ��� ����
#define RUN_CROP_X 43 
#define RUN_CROP_Y 43
#define RUN_SRC_W 45
#define RUN_SRC_H 38
#define RUN_DEST_W 120
#define RUN_DEST_H 120

//���� �ڸ��� ���� 
#define JUMP_CROP_X 30
#define JUMP_CROP_Y 10
#define JUMP_SRC_W 60
#define JUMP_SRC_H 70
#define JUMP_DEST_W 150
#define JUMP_DEST_H 230
*/

// �޸��� �ڸ��� ����
#define RUN_CROP_X 43 
#define RUN_CROP_Y 13
#define RUN_SRC_W 128
#define RUN_SRC_H 128
#define RUN_DEST_W 150
#define RUN_DEST_H 150

//���� �ڸ��� ���� 
#define JUMP_CROP_X 30
#define JUMP_CROP_Y 10
#define JUMP_SRC_W 128
#define JUMP_SRC_H 128
#define JUMP_DEST_W 150
#define JUMP_DEST_H 150

//������ ��
#define MAX_RUN_FRAMES 9
#define MAX_JUMP_FRAMES 6
#define MAX_SLIDE_FRAMES 4

//플레이어 시작 위치
#define StartX 100
#define StartY 215 //SCREEN_HEIGHT - GROUND_HEIGHT - RUN_DEST_H

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
	int slideFrame;

	int jumpDirection;
	float jumpSpeed;
	float maxJumpHeight;

	//추가
	int hit_offset_x;
	int hit_offset_y;
	int hit_w;
	int hit_h;
	int hurtTimer;

	ALLEGRO_BITMAP* runSheet;
	ALLEGRO_BITMAP* jumpSheet;
	ALLEGRO_BITMAP* slideSheet;

}Player;

void init_player(Player* p);
void update_player(Player* p);
void draw_player(Player* p);
void destroy_player(Player* p);
//그려지는 좌표 통일
float get_player_draw_y(Player* p);
Rect get_player_hitbox(Player* p);
void draw_player_hitbox(Player* p);

#endif
