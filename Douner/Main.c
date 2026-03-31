#include "global.h"
#include "keyboard.h"
#include "mouse.h"
#include "menu.h"
#include "item.h"
#include "rank.h"
#include "Player.h"
#include "obstacle.h"
#include "game_over.h"
#include "Background.h"
#include "hud.h"
#include "user.h"
#include "text_input.h"

ALLEGRO_FONT* menu_font;

void on_start();
void on_exit();
void on_enter_name();
void on_ranking();
void on_continue();
void on_back_to_menu();

MENU_ITEM main_menu[] = {
    MENU_BUTTON("Start Game", on_start),
    MENU_BUTTON("Ranking", on_ranking),
    MENU_BUTTON("Exit", on_exit),
    MENU_END()
};

MENU_ITEM menu_paused[] = {
    MENU_BUTTON("Continue", on_continue),
    MENU_BUTTON("New Game", on_start), // Start Game 대신 New Game으로 이름 변경 가능
    MENU_BUTTON("Ranking", on_ranking),
    MENU_BUTTON("Exit", on_exit),
    MENU_END()
};

MENU_ITEM* current_menu = main_menu;
FILE* rank_file;
long frames;
bool done = false;
char message[100] = "";
bool is_restart = false;

GameScreen cur_screen = SCREEN_START;

static GameState game;

void on_start() {
    game_init(&game);
    game.difficulty = 1;
    game.lives = 3;
    hud_reset_popup();
    hud_trigger_popup(POPUP_START);
    is_restart = true;
    current_menu = menu_paused;
    cur_screen = SCREEN_PLAY; // 게임 시작

}

void on_exit() {
    done = true;    // 프로그램 종료
}

void on_enter_name() {
    cur_screen = SCREEN_NAME_INPUT;
}

void on_ranking() {
    rank_init();
    cur_screen = SCREEN_RANKING;
}

void on_continue() {
    cur_screen = SCREEN_PLAY;
}

void on_back_to_menu() {
    menu_init(current_menu);
	cur_screen = SCREEN_MENU;
}



int main() {
    if (!al_init()) return -1;
    must_init(al_init_primitives_addon(), "primitives_addon");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    must_init(al_init_image_addon(), "image_addon");
    must_init(al_init_font_addon(), "font");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    menu_font = al_create_builtin_font();

    hud_init();
    obstacle_init();

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    keyboard_init();

    item_init();

    menu_init(current_menu);

    game_init(&game); // game.hp = 100, game.score = 0 초기화 포함 

    Obstacle obs_pool[MAX_OBS];
    InitObstacles(obs_pool, MAX_OBS);
    SpawnManager spawner;
    InitSpawnManager(&spawner);
    
    //효과음
    //al_install_audio();
    //al_init_acodec_addon();
    //al_reserve_samples(5);
    ALLEGRO_SAMPLE* sfx_hit = 0;
    //ALLEGRO_SAMPLE* sfx_hit = al_load_sample("hit_sound.wav"); // 파일명 확인!
    //if (!sfx_hit) {
    //    printf("효과음을 로드할 수 없습니다!\n");
    //}

    TextInput text_input;
    text_input_init(&text_input);


    Player player;
    init_player(&player);

    Background bg;
    init_background(&bg);

    srand(time(NULL));
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    while (!done) {
        al_wait_for_event(queue, &event);

        keyboard_update(&event);
        mouse_update(&event);

        switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
            // --- 게임 로직 업데이트 ---
            switch (cur_screen) {

            case SCREEN_START:
            case SCREEN_MENU:
                update_background(&bg, 0);
                if (menu_update(current_menu) == MENU_EXIT) {
                    done = true;
                }
                break;

            case SCREEN_PLAY:

                if (is_restart) {
                    InitObstacles(obs_pool, MAX_OBS); // 화면에 남은 장애물 싹 지우기
                    InitSpawnManager(&spawner);       // 소환 타이머 리셋
                    item_init();

                    player.state = PLAYER_RUN;
                    player.y = player.baseY;
                    player.jumpDirection = 0;
                    player.hurtTimer = 0;
                    player.runFrame = 0;

                    bg.x = 0;   // 배경 스크롤 위치도 처음으로 되돌림
                    frames = 0; // 프레임(시간) 초기화

                    is_restart = false; // 청소 끝! 스위치 끄기
                }

                update_background(&bg, game.difficulty);

                if (hud_update_popup()) {
                }
                else {
                    item_update(&game, &player);
                    update_player(&player);

                    UpdateSpawning(&spawner, obs_pool, MAX_OBS, &game);
                    UpdateObstacles(obs_pool, MAX_OBS, GRAVITY, player.x);

                    item_collision_check(&game, &player);
                    obstacle_collision_check(&player, obs_pool, MAX_OBS, &game, sfx_hit);
                    hp_update(&game);

                    if (game.score >= 100 && game.difficulty == 1) {
                        game.difficulty = 2;
                        hud_trigger_popup(POPUP_NEXT); 
                        InitObstacles(obs_pool, MAX_OBS);
                        InitSpawnManager(&spawner);
                    }
                    else if (game.score >= 200 && game.difficulty == 2) {
                        game.difficulty = 3;
                        hud_trigger_popup(POPUP_FINAL); 
                        InitObstacles(obs_pool, MAX_OBS);
                        InitSpawnManager(&spawner);
                    }
                }
               
                if(game.hp <= 0) {
                    game.lives--;
                    if (game.lives > 0) {
                        game.hp = 100;
                        InitObstacles(obs_pool, MAX_OBS);
                        InitSpawnManager(&spawner);
                    }
                    else {
                    Record new_record;
                    strncpy(new_record.name, game.player_name, sizeof(game.player_name) - 1);
					new_record.name[sizeof(game.player_name) - 1] = '\0';
					new_record.score = game.score;
					new_record.difficulty = game.difficulty;

					file_write(new_record);
					game_over_init(game.score);
                    current_menu = main_menu;
					cur_screen = SCREEN_GAME_OVER;
                    }
				}
                // 전역 키보드 배열(key)이 업데이트된다고 가정
                if (key[ALLEGRO_KEY_ESCAPE]) {
					on_back_to_menu();
                }
                break;
            case SCREEN_RANKING:
                rank_update();
                // 랭킹 화면 업데이트 로직 처리
                break;

            case SCREEN_GAME_OVER:
                game_over_update();
                // 게임 오버 화면 업데이트 로직 처리
                break;
            }

            redraw = true;
            frames++;
            mouse_tick();

            break; // closes case ALLEGRO_EVENT_TIMER

        case ALLEGRO_EVENT_KEY_CHAR:

            if (cur_screen == SCREEN_NAME_INPUT) {
                text_input_update(&text_input, &event);

                if (text_input.done) {
                    char* name = text_input.buffer;

                    if (user_exists(name)) {
                        snprintf(message, sizeof(message), "Welcome back, %s!", name);
                    }
                    else {
                        // 신규 유저
                        add_user(name);
                        snprintf(message, sizeof(message), "New user: %s", name);
                    }
                    strcpy(game.player_name, name);

                    cur_screen = SCREEN_MENU;

                    keyboard_init();
                }
            }
            break;

        case ALLEGRO_EVENT_KEY_DOWN:

            switch (cur_screen) {

            case SCREEN_START:
                cur_screen = SCREEN_NAME_INPUT;
                break;

            case SCREEN_PLAY:
                if (key_pressed(ALLEGRO_KEY_UP)) { //event.keyboard.keycode == ALLEGRO_KEY_UP
                    if (player.state == PLAYER_RUN) {
                        player.state = PLAYER_JUMP;
                        player.jumpDirection = 1;
                        player.jumpFrame = 0;
                    }
                }
                else if (key_pressed(ALLEGRO_KEY_DOWN)) { //event.keyboard.keycode == ALLEGRO_KEY_DOWN
                    if (player.state == PLAYER_RUN) {
                        player.state = PLAYER_SLIDING;
                        player.slideFrame = 0;
                    }
                }
                break;
            
            default:
                break;
            }

            break;

        case ALLEGRO_EVENT_KEY_UP:
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                if (player.state == PLAYER_SLIDING) {
                    player.state = PLAYER_RUN;
                    player.runFrame = 0;
                }
            }
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        } // event switch 종료

        if (done) break;

        // --- 그리기 로직 (루프 내부로 위치 수정) ---
        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0)); // 화면 초기화 필수 
            switch (cur_screen) {

            case SCREEN_START:
                draw_background(&bg);
                al_draw_text(menu_font, al_map_rgb(255, 255, 255),
                    SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
                    ALLEGRO_ALIGN_CENTER,
                    "Press Enter Key to Start");
                break;


            case SCREEN_MENU:
                draw_background(&bg);
                menu_draw(current_menu);
                al_draw_text(menu_font, al_map_rgb(255, 255, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, message);

                break;

            case SCREEN_PLAY:

                //draw_map();
                draw_background(&bg);
                DrawObstaclesWithImage(&game, obs_pool, MAX_OBS);
                draw_player(&player);
                //debug
                //draw_player_hitbox(&player);
                item_draw();
                draw_player_hitbox(&player);
                draw_obstacle_hitboxes(obs_pool, MAX_OBS);

                draw_item_hitboxes();
                hud_draw(&game);
                hud_draw_popup();
                break;

            case SCREEN_NAME_INPUT:
                draw_background(&bg);
                text_input_draw(&text_input);
                break;

            case SCREEN_RANKING:
                rank_draw();
                break;

            case SCREEN_GAME_OVER:
                game_over_draw();
                break;

            }

            al_flip_display();
            redraw = false;
        }
    } // while(!done) 루프 종료
    // --- 정리 및 자원 해제 (반드시 while 루프 밖에 위치해야 함) ---
    destroy_player(&player);
    hud_deinit();
    item_deinit();
    obstacle_deinit();
    al_destroy_timer(timer);
    al_destroy_font(menu_font);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    destroy_background(&bg);

    return 0;
}