#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h> // 도형 그리기를 위해 필요
#include <allegro5/allegro_image.h>
#define SCREEN_W 640
#define SCREEN_H 400
#define FPS 60.0f
#define GROUND_Y 350.0f
#define GRAVITY 0.5f
#define MAX_OBS 99
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

// 장애물 구조체
typedef struct {
    float x, y;
    float width, height;
    float speed;
    int is_active;
    ObstacleType type;

    // 점프 로직용
    float vy;
    float jump_power;
    float initial_y;
    int is_jumping;
    int has_jumped; // ★ 추가: 이미 점프를 시도했는지 체크하는 변수
    ALLEGRO_COLOR color; // Allegro 색상 정보 저장
} Obstacle;

// 함수 선언 (다른 파일에서 이 함수들을 호출함)
void InitObstacles(Obstacle obs[], int size);
void UpdateObstacles(Obstacle obs[], int size, float gravity, float player_x);
void DrawObstacles(Obstacle obs[], int size);
void SpawnObstacle(Obstacle obs[], ObstacleType type);
void DrawObstaclesWithImage(Obstacle obs[], int size);
void InitSpawnManager(SpawnManager* sm);
void UpdateSpawning(SpawnManager* sm, Obstacle obs[], int size);
#endif