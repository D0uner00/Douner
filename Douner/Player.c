#include "Player.h"

void init_player(Player* p) {
    p->runSheet = al_load_bitmap("male_hero-run.png");
    p->jumpSheet = al_load_bitmap("male_hero-jump.png");

    if (!p->runSheet || !p->jumpSheet) { // 둘 중 하나라도 로드 실패 시 종료
        fprintf(stderr, "이미지 로드 실패!\n");
        return -1;
    }

    p->runFrame = 0;
    p->jumpFrame = 0;
    p->x = StartX;
    p->baseY = StartY;
    p->y = p->baseY;
    p->isJumping = false;
    p->jumpDirection = 0;
    p->jumpSpeed = 15.0f;
    p->maxJumpHeight = 120;
}

void update_player(Player* p) {
    // 애니메이션 재생 로직
    if (!p->isJumping) {
        // 달리기
        p->runFrame = (p->runFrame + 1) % MAX_RUN_FRAMES;
    }
    else {//점프
        p->jumpFrame = (p->jumpFrame + 1);
        if (p->jumpFrame >= MAX_JUMP_FRAMES) p->jumpFrame = MAX_JUMP_FRAMES - 1; // 마지막 프레임 고정 (landing)
    }

    // 점프 Y축 물리 로직 (기존 유지)
    if (p->isJumping) {
        if (p->jumpDirection == 1) {
            p->y -= p->jumpSpeed;
            if (p->y <= p->baseY - p->maxJumpHeight) {
                p->y = p->baseY - p->maxJumpHeight;
                p->jumpDirection = -1;
            }
        }
        else if (p->jumpDirection == -1) {
            p->y += p->jumpSpeed;
            if (p->y >= p->baseY) {
                p->y = p->baseY;
                p->isJumping = false;
                p->jumpDirection = 0;
                p->jumpFrame = 0; // 착지 시 초기화
            }
        }
    }
}

void draw_player(Player* p) {
    // 프레임별 시트 변경
    if (!p->isJumping) {
        // 달리기
        int frameStartX = p->runFrame * 128;
        al_draw_scaled_bitmap(p->runSheet,
            frameStartX + RUN_CROP_X, RUN_CROP_Y, RUN_SRC_W, RUN_SRC_H,
            p->x, p->y, RUN_DEST_W, RUN_DEST_H, 0);
    }
    else {
        //점프
        int frameStartX = p->jumpFrame * 128;

        // ★중요 Y 보정★: 달리기 그림(130높이) 기준으로 Y 좌표가 계산되어 있습니다.
        // 더 큰 점프 그림(260높이)을 그리면 발이 땅밑으로 파고듭니다.
        // 그릴 때만 머리 위치를 위로 보정해 줘야 합니다.
        // (보정값 = 점프그림높이 - 달리그림높이 = 260 - 130 = 130만큼 머리를 위로(-))
        float yDrawCompensation = JUMP_DEST_H - RUN_DEST_H;
        float compensatedY = p->y - yDrawCompensation;

        al_draw_scaled_bitmap(p->jumpSheet,
            frameStartX + JUMP_CROP_X, JUMP_CROP_Y, JUMP_SRC_W, JUMP_SRC_H,
            p->x, compensatedY, JUMP_DEST_W, JUMP_DEST_H, 0);
    }
}

void destroy_player(Player* p) {
    al_destroy_bitmap(p->runSheet);
    al_destroy_bitmap(p->jumpSheet); // 메모리 해제 추가
}

//      임시      //
void draw_map() {
    al_clear_to_color(al_map_rgb(135, 206, 235));
    al_draw_filled_rectangle(0, SCREEN_HEIGHT - GROUND_HEIGHT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        al_map_rgb(34, 139, 34));
}
