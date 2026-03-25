#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "global.h"
// 1. 원본 달리기 자르기 정보 (유지)
#define RUN_CROP_X 43 
#define RUN_CROP_Y 33
#define RUN_SRC_W 45
#define RUN_SRC_H 45
#define RUN_DEST_W 105
#define RUN_DEST_H 130 

// ★ 2. 점프 시트 자르기 정보 (새로 추정해서 입력)
// 점프 시트의 캐릭터들이 키가 더 크고 넓기 때문에 자르기 영역을 더 크게 잡아야 합니다.
// (실행해보고 눈으로 확인하며 미세조정이 필요합니다)
#define JUMP_CROP_X 32
#define JUMP_CROP_Y 10
#define JUMP_SRC_W 64
#define JUMP_SRC_H 90 // 달리기보다 더 크고 긴 영역을 자릅니다.

// 달리기 비율에 맞춰 점프 시트를 화면에 뻥튀기할 크기
#define JUMP_DEST_W 140
#define JUMP_DEST_H 260 // 달리기보다 약 2배 크게 그림

#define MAX_RUN_FRAMES 10
#define MAX_JUMP_FRAMES 6 // 점프 시트도 10프레임으로 보임

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

void init_player(Player* p, float startX, float startY);
void update_player(Player* p);
void draw_player(Player* p);
void destroy_player(Player* p);

//		임시			//
void draw_map();
#endif // !_PLAYER_H_
