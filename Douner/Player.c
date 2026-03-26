#include "Player.h"

void init_player(Player* p) {
    p->runSheet = al_load_bitmap("dooner_run.png");
    p->jumpSheet = al_load_bitmap("dooner_jump.png");

    if (!p->runSheet || !p->jumpSheet) { // 둘 중 하나라도 로드 실패 시 종료
        fprintf(stderr, "이미지 로드 실패!\n");
        return ;
    }

    p->runFrame = 0;
    p->jumpFrame = 0;

    p->x = StartX;
    p->baseY = StartY;
    p->y = p->baseY;

    p->jumpDirection = 0;
    p->jumpSpeed = 15.0f;
    p->maxJumpHeight = 120; 

    p->hurtTimer = 0;

    p->state = PLAYER_RUN;
}

void update_player(Player* p) { // 데이터 및 상태 관리

    if (p->hurtTimer > 0) {
        p->hurtTimer--;
        if (p->hurtTimer <= 0) {
            p->state = (p->jumpDirection != 0) ? PLAYER_JUMP : PLAYER_RUN;
        }
    }
    
    switch (p->state) {

    case PLAYER_RUN:
        p->runFrame = (p->runFrame + 1) % MAX_RUN_FRAMES;
        break;

    case PLAYER_JUMP:
        p->jumpFrame++;
        if (p->jumpFrame >= MAX_JUMP_FRAMES)
            p->jumpFrame = MAX_JUMP_FRAMES - 1;

        if (p->jumpDirection == 1) { //상승
            p->y -= p->jumpSpeed;
            if (p->y <= p->baseY - p->maxJumpHeight) {
                p->y = p->baseY - p->maxJumpHeight;
                p->jumpDirection = -1;
            }
        }
        else { //하강
            p->y += p->jumpSpeed;
            if (p->y >= p->baseY) {
                p->y = p->baseY;
                p->state = PLAYER_RUN;
                p->jumpFrame = 0;
            }
        }
        break;
    }    
}

void draw_player(Player* p) { //그래픽 출력

    ALLEGRO_COLOR tint = al_map_rgb(255, 255, 255); // 기본 원본색
    if (p->hurtTimer > 0) {
        if ((p->hurtTimer % 4) < 2) { // 4프레임 주기로 빨강/원본 깜빡임
            tint = al_map_rgb(255, 0, 0);
        }
    }

    switch (p->state) {

    case PLAYER_RUN: {
        int frameStartX = p->runFrame * 128;
        al_draw_tinted_scaled_bitmap(p->runSheet, tint,
            frameStartX + RUN_CROP_X, RUN_CROP_Y, RUN_SRC_W, RUN_SRC_H,
            p->x, p->y, RUN_DEST_W, RUN_DEST_H, 0);
        break;
    }

    case PLAYER_JUMP: {
        int frameStartX = p->jumpFrame * 128;
        //보정값 = 점프그림높이 - 달리그림높이 = 260 - 130 = 130만큼 머리를 위로(-)
        float yComp = JUMP_DEST_H - RUN_DEST_H;
        float y = p->y - yComp;

        al_draw_tinted_scaled_bitmap(p->jumpSheet, tint,
            frameStartX + JUMP_CROP_X, JUMP_CROP_Y, JUMP_SRC_W, JUMP_SRC_H,
            p->x, y, JUMP_DEST_W, JUMP_DEST_H, 0);
        break;
    }

    }
}

void destroy_player(Player* p) {
    al_destroy_bitmap(p->runSheet);
    al_destroy_bitmap(p->jumpSheet); // 메모리 해제 추가
}



