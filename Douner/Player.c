#include "global.h"
#include "Player.h"

void init_player(Player* p) {
    p->runSheet = al_load_bitmap("male_hero-run.png");
    p->jumpSheet = al_load_bitmap("male_hero-jump.png");

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

    p->state = PLAYER_RUN;

    //히트박스 추가
    p->hit_offset_x = 30;
    p->hit_offset_y = 8;
    p->hit_w = 70;
    p->hit_h = 110;
}

void update_player(Player* p) { // 데이터 및 상태 관리
    
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
//그려지는 좌표 통일
float get_player_draw_y(Player* p)
{
    if (p->state == PLAYER_JUMP)
        return p->y - (JUMP_DEST_H - RUN_DEST_H);

    return p->y;
}

void draw_player(Player* p) { //그래픽 출력

    float y = get_player_draw_y(p);

    switch (p->state) {

    case PLAYER_RUN: {
        int frameStartX = p->runFrame * 128;

        //float y = get_player_draw_y(p);
        al_draw_scaled_bitmap(p->runSheet,
            frameStartX + RUN_CROP_X, RUN_CROP_Y, RUN_SRC_W, RUN_SRC_H,
            p->x, y, RUN_DEST_W, RUN_DEST_H, 0);
        break;
    }

    case PLAYER_JUMP: {
        int frameStartX = p->jumpFrame * 128;

        //보정값 = 점프그림높이 - 달리그림높이 = 260 - 130 = 130만큼 머리를 위로(-)
        //float yComp = JUMP_DEST_H - RUN_DEST_H;
        //float y = p->y - yComp;

        //그려지는 좌표 통일
        //float y = get_player_draw_y(p);

        al_draw_scaled_bitmap(p->jumpSheet,
            frameStartX + JUMP_CROP_X, JUMP_CROP_Y, JUMP_SRC_W, JUMP_SRC_H,
            p->x, y, JUMP_DEST_W, JUMP_DEST_H, 0);
        break;
    }

    case PLAYER_HURT:
        // 나중에 추가 가능 (빨간색, 깜빡임 등)
        break;
    }
}

//hit box 추가
Rect get_player_hitbox(Player* p)
{
    Rect r;

    float drawY = get_player_draw_y(p);

    /*
    // 점프 시 보정 (draw 기준으로 맞추기)
    if (p->state == PLAYER_JUMP)
    {
        drawY -= (JUMP_DEST_H - RUN_DEST_H);
    }*/

    // offset 적용 (몸통 기준으로 이동)
    r.x = (int)p->x + p->hit_offset_x;
    r.y = (int)drawY + p->hit_offset_y;

    // 크기 적용
    r.w = p->hit_w;
    r.h = p->hit_h;

    return r;
}


//hit box 디버깅용
void draw_player_hitbox(Player* p)
{
    Rect r = get_player_hitbox(p);

    al_draw_rectangle(
        r.x, r.y,
        r.x + r.w,
        r.y + r.h,
        al_map_rgb(255, 0, 0),
        2
    );
}

void destroy_player(Player* p) {
    al_destroy_bitmap(p->runSheet);
    al_destroy_bitmap(p->jumpSheet); // 메모리 해제 추가
}

