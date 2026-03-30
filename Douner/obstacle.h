#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "global.h"
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

// 함수 선언 (다른 파일에서 이 함수들을 호출함)
void InitObstacles(Obstacle obs[], int size);
void UpdateObstacles(Obstacle obs[], int size, float gravity, float player_x);
void SpawnObstacle(Obstacle* obs, ObstacleType type, GameState* game);
void DrawObstaclesWithImage(GameState* game, Obstacle obs[], int size, ALLEGRO_BITMAP* img_g, ALLEGRO_BITMAP* img_f, ALLEGRO_BITMAP* img_t,
    ALLEGRO_BITMAP* tornado, ALLEGRO_BITMAP* kirby, ALLEGRO_BITMAP* kirby_jump, ALLEGRO_BITMAP* img_meteor, ALLEGRO_BITMAP* img_amongus, ALLEGRO_BITMAP* img_amongus_jump, ALLEGRO_BITMAP* img_keroro);
void InitSpawnManager(SpawnManager* sm);
void UpdateSpawning(SpawnManager* sm, Obstacle obs[], int size, GameState* Game);
void obstacle_collision_check(Player* player, Obstacle obs[], int size, GameState* game, ALLEGRO_SAMPLE* sfx_hit);
#endif