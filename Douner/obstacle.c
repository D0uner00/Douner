#include "obstacle.h"
// 초기화: 장애물들을 화면 밖 대기 상태로 설정
void InitObstacles(Obstacle obs[], int size) {
    for (int i = 0; i < size; i++) {
        obs[i].is_active = 0;
        obs[i].width = 40;
        obs[i].height = 40;
        obs[i].color = al_map_rgb(255, 0, 0); // 기본 빨간색
    }
}

// 업데이트: 이동 및 점프 로직
void UpdateObstacles(Obstacle obs[], int size, float gravity, float player_x) {
    for (int i = 0; i < size; i++) {
        if (!obs[i].is_active) continue;

        // 1. 왼쪽으로 이동
        obs[i].x -= obs[i].speed;

        // 2. 점프 타입일 경우 로직 처리
        if (obs[i].type == OBS_JUMPING) {
            // 플레이어가 근처에 오면 점프 시작 (예: 거리 100 이하)
            if (!obs[i].is_jumping && obs[i].x - player_x < 50) {
                obs[i].is_jumping = 1;
                obs[i].vy = obs[i].jump_power;
            }

            if (obs[i].is_jumping) {
                obs[i].vy += gravity;
                obs[i].y += obs[i].vy;

                if (obs[i].y >= obs[i].initial_y) {
                    obs[i].y = obs[i].initial_y;
                    obs[i].is_jumping = 0;
                }
            }
        }

        // 3. 화면 밖으로 나가면 비활성화
        if (obs[i].x + obs[i].width < 0) {
            obs[i].is_active = 0;
        }
    }
}

// 그리기: Allegro 함수 사용
void DrawObstacles(Obstacle obs[], int size) {
    for (int i = 0; i < size; i++) {
        if (obs[i].is_active) {
            al_draw_filled_rectangle(obs[i].x, obs[i].y,
                obs[i].x + obs[i].width,
                obs[i].y + obs[i].height,
                obs[i].color);
        }
    }
}
void SpawnObstacle(Obstacle* obs, ObstacleType type) {
    obs->width = 40;
    obs->height = 40;
    obs->x = SCREEN_W;

    // 타입별 초기 높이 설정
    if (type == OBS_FLYING) {
        obs->y = GROUND_Y - 70; // 공중 장애물
    }
    else {
        obs->y = GROUND_Y - obs->height; // 바닥 장애물
    }

    obs->initial_y = obs->y;
    obs->speed = 4.0f; // 속도 랜덤
    obs->is_active = 1;
    obs->is_jumping = 0;
    obs->has_jumped = 0;
    obs->vy = 0;
    obs->type = type;
    obs->jump_power = -10.0f;

    // 타입별 색상 구분
    if (type == OBS_GROUND) obs->color = al_map_rgb(255, 0, 0);      // 빨강
    if (type == OBS_FLYING) obs->color = al_map_rgb(0, 0, 255);      // 파랑
    if (type == OBS_JUMPING) obs->color = al_map_rgb(0, 255, 0);     // 초록
}
void DrawObstaclesWithImage(Obstacle obs[], int size) {
    for (int i = 0; i < size; i++) {
        if (obs[i].is_active) {
            ALLEGRO_BITMAP* img_g = al_load_bitmap("trash.png");
            ALLEGRO_BITMAP* img_f = al_load_bitmap("dish.png");
            ALLEGRO_BITMAP* img_j = al_load_bitmap("dish.png");
            ALLEGRO_BITMAP* current_img = NULL;

            // 타입에 맞는 이미지 선택
            if (obs[i].type == OBS_GROUND) current_img = img_g;
            else if (obs[i].type == OBS_FLYING) current_img = img_f;
            else if (obs[i].type == OBS_JUMPING) current_img = img_j;

            if (current_img) {
                // 이미지를 장애물 크기에 딱 맞춰서 그립니다.
                al_draw_scaled_bitmap(current_img,
                    0, 0, al_get_bitmap_width(current_img), al_get_bitmap_height(current_img), // 원본 영역
                    obs[i].x, obs[i].y, obs[i].width, obs[i].height, // 화면에 그려질 위치와 크기
                    0); // 플래그 (0)
            }
        }
    }
}
