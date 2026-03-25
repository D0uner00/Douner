#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "obstacle.h" // 작성하신 헤더 파일


int main() {
    // 1. Allegro 초기화
    if (!al_init()) return -1;
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    // 2. 변수 준비
    Obstacle obs_pool[MAX_OBS];
    InitObstacles(obs_pool, MAX_OBS);

    float player_x = 100; // 플레이어의 현재 위치 (장애물 점프 감지용)
    srand(time(NULL));

    int redraw = 1;
    al_start_timer(timer);

    // 3. 게임 루프
    while (1) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // 스페이스바 누르면 랜덤한 장애물 하나 활성화
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                for (int i = 0; i < MAX_OBS; i++) {
                    if (!obs_pool[i].is_active) {
                        SpawnObstacle(&obs_pool[i], rand() % 3);
                        break;
                    }
                }
            }
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            // 키보드 입력으로 플레이어 이동 (점프 장애물 테스트용)
            ALLEGRO_KEYBOARD_STATE ks;
            al_get_keyboard_state(&ks);
            if (al_key_down(&ks, ALLEGRO_KEY_LEFT)) player_x -= 5;
            if (al_key_down(&ks, ALLEGRO_KEY_RIGHT)) player_x += 5;

            // 장애물 업데이트
            UpdateObstacles(obs_pool, MAX_OBS, GRAVITY, player_x);
            redraw = 1;
        }

        // 4. 그리기
        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(200, 200, 200)); // 배경 회색

            // 바닥 선
            al_draw_line(0, GROUND_Y, SCREEN_W, GROUND_Y, al_map_rgb(0, 0, 0), 2);

            // 플레이어 표시 (단순한 원)
            al_draw_filled_circle(player_x, GROUND_Y - 20, 20, al_map_rgb(50, 50, 50));

            // 장애물 그리기
            DrawObstaclesWithImage(obs_pool, MAX_OBS);

            // 가이드 텍스트 (알레그로 폰트 생략, 원리로 이해)
            // 실제 구현 시엔 al_draw_text 사용 권장

            al_flip_display();
            redraw = 0;
        }
    }

    // 5. 정리
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}