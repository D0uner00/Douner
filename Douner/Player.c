
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

#if 0
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
    if (!al_init()) { //알레그로 실행
        fprintf(stderr, "Allegro 초기화 실패!\n");
        return -1;
    }
    al_init_image_addon(); //이미지 기능
    al_init_primitives_addon(); //도형 기능
    al_install_keyboard(); //키보드 처리

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    //애니메이션 속도 임의 설정
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 15.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    //이벤트 큐의 연결
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    ALLEGRO_BITMAP* runSheet = al_load_bitmap("male_hero-run.png");
    if (!runSheet) {
        fprintf(stderr, "이미지 로드 실패! 파일 경로를 확인하세요.\n");
        return -1;
    }

    int currentFrame = 0;
    float charX = 100; //화면상 x좌표

    // 점프를 위한 변수
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
            if (!isJumping) {
                currentFrame = (currentFrame + 1) % MAX_FRAMES;
            }
            else currentFrame = 0;

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

#endif

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;
const int GROUND_HEIGHT = 70;

// 1. 원본 달리기 자르기 정보 (유지)
const int RUN_CROP_X = 43; const int RUN_CROP_Y = 33;
const int RUN_SRC_W = 45; const int RUN_SRC_H = 45;
const int RUN_DEST_W = 105; const int RUN_DEST_H = 130; // 화면에 그릴 달리기 높이

// ★ 2. 점프 시트 자르기 정보 (새로 추정해서 입력)
// 점프 시트의 캐릭터들이 키가 더 크고 넓기 때문에 자르기 영역을 더 크게 잡아야 합니다.
// (실행해보고 눈으로 확인하며 미세조정이 필요합니다)
const int JUMP_CROP_X = 32; const int JUMP_CROP_Y = 10;
const int JUMP_SRC_W = 64; const int JUMP_SRC_H = 90; // 달리기보다 더 크고 긴 영역을 자릅니다.

// 달리기 비율에 맞춰 점프 시트를 화면에 뻥튀기할 크기
const int JUMP_DEST_W = 140; const int JUMP_DEST_H = 260; // 달리기보다 약 2배 크게 그림

const int MAX_RUN_FRAMES = 10;
const int MAX_JUMP_FRAMES = 6; // 점프 시트도 10프레임으로 보임

int main() {
    if (!al_init()) return -1;
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
    // ★ 변경 1: 점프 이미지 로드
    ALLEGRO_BITMAP* jumpSheet = al_load_bitmap("male_hero-jump.png");

    if (!runSheet || !jumpSheet) { // 둘 중 하나라도 로드 실패 시 종료
        fprintf(stderr, "이미지 로드 실패!\n");
        return -1;
    }

    int runFrame = 0;   // 달리기 프레임 추적
    // ★ 변경 2: 점프 프레임 추적 변수 추가
    int jumpFrame = 0;
    float charX = 100;

    // Y축 물리 법칙 (기존 유지)
    float baseY = SCREEN_HEIGHT - GROUND_HEIGHT - RUN_DEST_H; // 땅에 닿아있을 때의 기본 Y 좌표 (상단 130기준)
    float charY = baseY;

    bool isJumping = false;
    int jumpDirection = 0;
    float jumpSpeed = 22.0f;
    float maxJumpHeight = 150;

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
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                if (!isJumping) {
                    isJumping = true;
                    jumpDirection = 1;
                    // ★ 변경: 점프 시작 시 프레임 초기화
                    jumpFrame = 0;
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            // ★ 변경 3: 애니메이션 재생 로직 분리
            if (!isJumping) {
                // 달리 중일 때만 달리기 프레임을 넘깁니다.
                runFrame = (runFrame + 1) % MAX_RUN_FRAMES;
            }
            else {
                // 점프 중일 때는 점프 전용 애니메이션을 재생합니다.
                // 위로 올라갈 때는 상승 모션(0~4 프레임), 내려올 때는 하강 모션(5~9 프레임)으로 고정해도 좋습니다.
                // 여기서는 일단 순차적으로 재생합니다.
                jumpFrame = (jumpFrame + 1);
                if (jumpFrame >= MAX_JUMP_FRAMES) jumpFrame = MAX_JUMP_FRAMES - 1; // 마지막 프레임 고정 (landing)
            }

            // 점프 Y축 물리 로직 (기존 유지)
            if (isJumping) {
                if (jumpDirection == 1) {
                    charY -= jumpSpeed;
                    if (charY <= baseY - maxJumpHeight) {
                        charY = baseY - maxJumpHeight;
                        jumpDirection = -1;
                    }
                }
                else if (jumpDirection == -1) {
                    charY += jumpSpeed;
                    if (charY >= baseY) {
                        charY = baseY;
                        isJumping = false;
                        jumpDirection = 0;
                        jumpFrame = 0; // 착지 시 초기화
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

            // ★ 변경 4: 상태에 따라 그리는 시트 변경
            if (!isJumping) {
                // [달리기 그리기] - 기존 로직 유지
                int frameStartX = runFrame * 128;
                al_draw_scaled_bitmap(runSheet,
                    frameStartX + RUN_CROP_X, RUN_CROP_Y, RUN_SRC_W, RUN_SRC_H,
                    charX, charY, RUN_DEST_W, RUN_DEST_H, 0);
            }
            else {
                // [점프 그리기] - 새로운 자르기 정보와 크기 사용
                int frameStartX = jumpFrame * 128;

                // ★중요 Y 보정★: 달리기 그림(130높이) 기준으로 Y 좌표가 계산되어 있습니다.
                // 더 큰 점프 그림(260높이)을 그리면 발이 땅밑으로 파고듭니다.
                // 그릴 때만 머리 위치를 위로 보정해 줘야 합니다.
                // (보정값 = 점프그림높이 - 달리그림높이 = 260 - 130 = 130만큼 머리를 위로(-))
                float yDrawCompensation = JUMP_DEST_H - RUN_DEST_H;
                float compensatedY = charY - yDrawCompensation;

                al_draw_scaled_bitmap(jumpSheet,
                    frameStartX + JUMP_CROP_X, JUMP_CROP_Y, JUMP_SRC_W, JUMP_SRC_H,
                    charX, compensatedY, JUMP_DEST_W, JUMP_DEST_H, 0);
            }

            al_flip_display();
        }
    }

    al_destroy_bitmap(runSheet);
    al_destroy_bitmap(jumpSheet); // 메모리 해제 추가
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}