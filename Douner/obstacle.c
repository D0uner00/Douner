#include "obstacle.h"
#include "player.h"
#include "global.h"
#include <stdlib.h> // rand() 사용을 위해 필요

// 1. 소환 관리자 초기화 (시작 전 1번 호출)
void InitSpawnManager(SpawnManager* sm) {
    sm->timer = 0;
    sm->interval = 120; // 처음에 2초(60fps 기준) 뒤 소환
}

// 2. 실시간 소환 로직 (타이머 이벤트마다 호출)
void UpdateSpawning(SpawnManager* sm, Obstacle obs[], int size) {
    sm->timer++;

    if (sm->timer >= sm->interval) {
        // 비어있는(비활성화된) 장애물 칸 찾기
        for (int i = 0; i < size; i++) {
            if (!obs[i].is_active) {
                // 랜덤하게 3가지 타입 중 하나 소환
                SpawnObstacle(&obs[i], (ObstacleType)(rand() % 3));
                break;
            }
        }
        // 타이머 리셋 및 다음 소환 간격 랜덤 설정 (1~3초 사이)
        sm->timer = 0;
        sm->interval = 60 + (rand() % 120);
    }
}

// 3. 장애물 배열 전체 초기화
void InitObstacles(Obstacle obs[], int size) {
    for (int i = 0; i < size; i++) {
        obs[i].is_active = 0;
    }
}

// 4. 장애물 이동/점프/애니메이션 업데이트
void UpdateObstacles(Obstacle obs[], int size, float gravity, float player_x) {
    for (int i = 0; i < size; i++) {
        if (!obs[i].is_active) continue;

        // 이동
        obs[i].x -= obs[i].speed;

        // 점프 로직
        if (obs[i].type == OBS_JUMPING) {
            if (!obs[i].is_jumping && (obs[i].x - player_x < 150)) {
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

        // 애니메이션 프레임 계산
        obs[i].frame_timer--;
        if (obs[i].frame_timer <= 0) {
            obs[i].cur_frame = (obs[i].cur_frame + 1) % 4; // 0~3 무한반복
            obs[i].frame_timer = obs[i].frame_delay;
        }

        // 화면 밖 소멸
        if (obs[i].x + obs[i].width < 0) {
            obs[i].is_active = 0;
        }
    }
}

// 5. 장애물 개별 소환 설정
void SpawnObstacle(Obstacle* obs, ObstacleType type) {
    obs->is_active = 1;
    obs->type = type;
    obs->x = SCREEN_WIDTH;
    obs->speed = 5.0f;

    // 규격 설정 (쓰레기통은 90, 나머지는 40)
    if (type == OBS_GROUND) {         // 쓰레기통
        obs->width = 40;
        obs->height = 50;
    }
    else if (type == OBS_FLYING) {    // 접시 (비행)
        obs->width = 60;
        obs->height = 40;
    }
    else if (type == OBS_JUMPING) {   // 트롤 (점프)
        obs->width = 40;
        obs->height = 100;
    }
    // Y 좌표 설정
    if (type == OBS_FLYING) obs->y = GROUND_Y - 120;
    else if (type == OBS_GROUND) obs->y = GROUND_Y - obs->height - 20;
    else obs->y = GROUND_Y - obs->height;

    obs->initial_y = obs->y;
    obs->vy = 0;
    obs->is_jumping = 0;
    obs->jump_power = -12.0f;

    // 애니메이션 초기화
    obs->cur_frame = 0;
    obs->frame_delay = 8;
    obs->frame_timer = obs->frame_delay;
}

// 6. 그리기 (이미지 포인터 사용)
void DrawObstaclesWithImage(Obstacle obs[], int size, ALLEGRO_BITMAP* img_g, ALLEGRO_BITMAP* img_f, ALLEGRO_BITMAP* img_j_sheet) {
    for (int i = 0; i < size; i++) {
        if (!obs[i].is_active) continue;

        if (obs[i].type == OBS_JUMPING && img_j_sheet) {
            int sw = al_get_bitmap_width(img_j_sheet) / 4;
            int sh = al_get_bitmap_height(img_j_sheet);
            al_draw_scaled_bitmap(img_j_sheet, obs[i].cur_frame * sw, 0, sw, sh,
                obs[i].x, obs[i].y, obs[i].width, obs[i].height, 0);
        }
        else {
            ALLEGRO_BITMAP* cur = (obs[i].type == OBS_GROUND) ? img_g : img_f;
            if (cur) {
                al_draw_scaled_bitmap(cur, 0, 0, al_get_bitmap_width(cur), al_get_bitmap_height(cur),
                    obs[i].x, obs[i].y, obs[i].width, obs[i].height, 0);
            }
        }
    }
}

void obstacle_collision_check(Player* player, Obstacle obs[], int size, GameState* game)
{
    Rect pBox = get_player_hitbox(player);
    // 이미 아픈 상태이거나 죽은 상태라면 중복 충돌 방지 (선택 사항)
    if (player->hurtTimer > 0 || player->state == PLAYER_DEATH) return;
    for (int i = 0; i < size; i++) {
        if (!obs[i].is_active) continue;

        // 히트박스 설정 (상태에 따른 플레이어 크기)
        Rect iBox = {
           obs[i].x,
           obs[i].y,
           20,
           20
        };

        // AABB 충돌 계산
        if (collide_rect(pBox, iBox))
        {
            printf("Dameged! life = %d\n", --game->life);
            // 라이프 감소
            obs[i].is_active = 0;       // 장애물 제거

            player->hurtTimer = 30; // 피격 상태로 전환

        }
    }
}