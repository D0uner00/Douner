#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "obstacle.h" // �ۼ��Ͻ� ��� ����


int main() {
    // 1. Allegro �ʱ�ȭ
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

    // 2. ���� �غ�
    Obstacle obs_pool[MAX_OBS];
    InitObstacles(obs_pool, MAX_OBS);
    SpawnManager spawner;
    InitSpawnManager(&spawner);

    float player_x = 100; // �÷��̾��� ���� ��ġ (��ֹ� ���� ������)
    srand(time(NULL));

    int redraw = 1;
    al_start_timer(timer);

    // 3. ���� ����
    while (1) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;



        // ... 게임 루프 내부 ...
        if (event.type == ALLEGRO_EVENT_TIMER) {
            // 2. 관리자에게 "시간 흘렀으니까 알아서 소환해"라고 시킴
            UpdateSpawning(&spawner, obs_pool, MAX_OBS);
        }
        redraw = 1;


        if (event.type == ALLEGRO_EVENT_TIMER) {
            // Ű���� �Է����� �÷��̾� �̵� (���� ��ֹ� �׽�Ʈ��)
            ALLEGRO_KEYBOARD_STATE ks;
            al_get_keyboard_state(&ks);
            if (al_key_down(&ks, ALLEGRO_KEY_LEFT)) player_x -= 5;
            if (al_key_down(&ks, ALLEGRO_KEY_RIGHT)) player_x += 5;

            // ��ֹ� ������Ʈ
            UpdateObstacles(obs_pool, MAX_OBS, GRAVITY, player_x);
            redraw = 1;
        }

        // 4. �׸���
        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(200, 200, 200)); // ��� ȸ��

            // �ٴ� ��
            al_draw_line(0, GROUND_Y, SCREEN_W, GROUND_Y, al_map_rgb(0, 0, 0), 2);

            // �÷��̾� ǥ�� (�ܼ��� ��)
            al_draw_filled_circle(player_x, GROUND_Y - 20, 20, al_map_rgb(50, 50, 50));

            // ��ֹ� �׸���
            DrawObstaclesWithImage(obs_pool, MAX_OBS);

            // ���̵� �ؽ�Ʈ (�˷��׷� ��Ʈ ����, ������ ����)
            // ���� ���� �ÿ� al_draw_text ��� ����

            al_flip_display();
            redraw = 0;
        }
    }

    // 5. ����
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}