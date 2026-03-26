#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h> // 도형 그리기를 위해 필요
#include <allegro5/allegro_image.h>
#define SCREEN_W SCREEN_WIDTH
#define SCREEN_H SCREEN_HEIGHT
#define FPS 60.0f
#define GROUND_Y 350.0f
#define GRAVITY 0.5f
#define MAX_OBS 99
#include "player.h"
typedef struct {
    int timer;
    int interval;
} SpawnManager;

// 장애물 종류
typedef enum {
    OBS_GROUND,
    OBS_FLYING,
    OBS_JUMPING
} ObstacleType;

typedef struct {
    float x, y;
    float width, height;
    float speed;
    int is_active;
    ALLEGRO_COLOR color;
    int has_jumped;
    ObstacleType type;

    float vy;
    float jump_power;
    float initial_y;
    int is_jumping;

    // ★ 애니메이션 변수 반드시 추가
    int cur_frame;      // 현재 0~3
    int frame_timer;    // 카운터
    int frame_delay;    // 속도 기준
} Obstacle;
// main 함수 내부 (루프 시작 전)
void Loadimage();
// 함수 선언 (다른 파일에서 이 함수들을 호출함)
void InitObstacles(Obstacle obs[], int size);
void UpdateObstacles(Obstacle obs[], int size, float gravity, float player_x);
void DrawObstacles(Obstacle obs[], int size);
void SpawnObstacle(Obstacle obs[], ObstacleType type);
void DrawObstaclesWithImage(Obstacle obs[], int size, ALLEGRO_BITMAP* img_g, ALLEGRO_BITMAP* img_f, ALLEGRO_BITMAP* img_t);
void InitSpawnManager(SpawnManager* sm);
void UpdateSpawning(SpawnManager* sm, Obstacle obs[], int size);
// obstacle.h
// 플레이어의 좌표와 크기를 받아 충돌 여부를 확인하고, 충돌 시 장애물을 제거하는 함수
void obstacle_collision_check(Player* player, Obstacle obs[], int size);
#endif