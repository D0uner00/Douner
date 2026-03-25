
#if 0

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> // 도형(땅)을 그리기 위해 필요

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;
const int GROUND_HEIGHT = 70;

// 캐릭터 원본 및 출력 크기 설정
const int FRAME_WIDTH = 128;
const int FRAME_HEIGHT = 128;
const int CROP_X = 34;
const int CROP_Y = 28;
const int SRC_W = 60;
const int SRC_H = 72;
const int DEST_W =100;
const int DEST_H = 200;
const int MAX_FRAMES = 10;

int main() {
    // 1. Allegro 초기화
    if (!al_init()) {
        fprintf(stderr, "Allegro 초기화 실패!\n");
        return -1;
    }
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    // 2. 디스플레이, 타이머, 이벤트 큐 생성
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 15.0); // 1초에 15프레임 (달리기 속도 조절)
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // 3. 이미지 로드 (반드시 실행 파일과 같은 폴더에 male_hero-run.png가 있어야 함)
    ALLEGRO_BITMAP* runSheet = al_load_bitmap("male_hero-run.png");
    if (!runSheet) {
        fprintf(stderr, "이미지 로드 실패! 파일 경로를 확인하세요.\n");
        return -1;
    }

    // 캐릭터 초기 변수
    int currentFrame = 0;
    float charX = 100; // 화면 왼쪽에서 약간 떨어진 위치
    float yOffset = 55;
    float charY = SCREEN_HEIGHT - GROUND_HEIGHT - DEST_H + yOffset; // 400 - 30 - 100 = 270

    bool done = false;
    bool redraw = true;

    al_start_timer(timer);

    // 4. 메인 게임 루프
    while (!done) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        // 창 닫기 버튼이나 ESC 키를 누르면 종료
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) done = true;
        }
        // 타이머 이벤트: 프레임 증가
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            currentFrame = (currentFrame + 1) % MAX_FRAMES; // 0~9 프레임 반복
            redraw = true;
        }

        // 렌더링 (화면 그리기)
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            // 배경 그리기 (하늘색)
            al_clear_to_color(al_map_rgb(135, 206, 235));

            // 땅 그리기 (초록색, y좌표 370부터 400까지)
            al_draw_filled_rectangle(0, SCREEN_HEIGHT - GROUND_HEIGHT,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                al_map_rgb(34, 139, 34));

            // 현재 프레임의 X 시작 좌표 계산
            int frameStartX = currentFrame * FRAME_WIDTH;

            // 캐릭터 스케일링하여 그리기
            al_draw_scaled_bitmap(
                runSheet,
                frameStartX + CROP_X, CROP_Y,  // 원본 X, Y
                SRC_W, SRC_H,                  // 자를 크기
                charX, charY,                  // 그릴 위치
                DEST_W, DEST_H,                // 늘릴 크기 (65x100)
                0                              // 방향 그대로
            );

            al_flip_display(); // 화면 업데이트
        }
    }

    // 5. 메모리 해제
    al_destroy_bitmap(runSheet);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}

#endif

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;
const int GROUND_HEIGHT = 70; //땅 높이

const int FRAME_WIDTH = 128;
const int FRAME_HEIGHT = 128;
const int CROP_X = 43;
const int CROP_Y = 33;
const int SRC_W = 45;
const int SRC_H = 45; 
//45x45 -> 120x120
const int DEST_W = 120;
const int DEST_H = 120;
const int MAX_FRAMES = 10;

int main() {
    if (!al_init()) {
        fprintf(stderr, "Allegro 초기화 실패!\n");
        return -1;
    }
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 15.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    ALLEGRO_BITMAP* runSheet = al_load_bitmap("male_hero-run.png");
    if (!runSheet) {
        fprintf(stderr, "이미지 로드 실패! 파일 경로를 확인하세요.\n");
        return -1;
    }

    int currentFrame = 0;
    float charX = 100;

    // 점프를 위한 변수들 추가
    float baseY = SCREEN_HEIGHT - GROUND_HEIGHT - DEST_H ; // 땅에 닿아있을 때의 기본 Y 좌표
    float charY = baseY;

    bool isJumping = false;
    int jumpDirection = 0;       // 1: 위로 올라감, -1: 아래로 내려옴
    float jumpSpeed = 22.0f;     // 점프 상승/하강 속도 (숫자가 클수록 빠름)
    float maxJumpHeight = 150;// 캐릭터 키(180)만큼 점프

    bool done = false;
    bool redraw = true;

    al_start_timer(timer);

    while (!done) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) done = true;

            // 스페이스바나 위쪽 화살표를 누르면 점프 시작
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                if (!isJumping) {
                    isJumping = true;
                    jumpDirection = 1; // 위로 이동 시작
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            currentFrame = (currentFrame + 1) % MAX_FRAMES;

            // 단순 점프 로직 (물리 법칙 제외)
            if (isJumping) {
                if (jumpDirection == 1) { // 올라가는 중
                    charY -= jumpSpeed;
                    if (charY <= baseY - maxJumpHeight) {
                        charY = baseY - maxJumpHeight; // 최대 높이에 도달하면 고정
                        jumpDirection = -1;            // 내려오기 시작
                    }
                }
                else if (jumpDirection == -1) { // 내려오는 중
                    charY += jumpSpeed;
                    if (charY >= baseY) {
                        charY = baseY;      // 땅에 착지하면 좌표를 원래대로 고정
                        isJumping = false;  // 점프 상태 해제
                        jumpDirection = 0;
                    }
                }
            }
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            al_clear_to_color(al_map_rgb(135, 206, 235));
            al_draw_filled_rectangle(0, SCREEN_HEIGHT - GROUND_HEIGHT,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                al_map_rgb(34, 139, 34));

            int frameStartX = currentFrame * FRAME_WIDTH;

            al_draw_scaled_bitmap(
                runSheet,
                frameStartX + CROP_X, CROP_Y,
                SRC_W, SRC_H,
                charX, charY,                  // 변경된 charY 적용
                DEST_W, DEST_H,
                0
            );

            al_flip_display();
        }
    }

    al_destroy_bitmap(runSheet);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}