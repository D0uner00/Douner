#include "global.h"
#include "keyboard.h"
#include "mouse.h"
#include "menu.h"
#include "hud.h"
#include "item.h"
#include "Player.h"
#include "obstacle.h"

ALLEGRO_FONT* menu_font;

long frames;
long score = 0;
bool done = false;
bool in_menu = true;

void on_start() {
    in_menu = false; // 게임 시작
}

void on_exit() {
    done = true;    // 프로그램 종료
}

MENU_ITEM main_menu[] = {
    MENU_BUTTON("TEMP",NULL),
    MENU_BUTTON("Start Game",on_start),
    MENU_BUTTON("Exit", on_exit),
    MENU_BUTTON("TEMP",NULL),
    MENU_END()
};

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
    //ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    /*
    Obstacle obs_pool[MAX_OBS];
    InitObstacles(obs_pool, MAX_OBS);
    SpawnManager spawner;
    InitSpawnManager(&spawner);
    */

    keyboard_init();
    item_init();
    menu_init(main_menu);

    GameState game;
    game_init(&game);
    // 장애물 준비 코드
    Obstacle obs_pool[MAX_OBS];
    InitObstacles(obs_pool, MAX_OBS);
    SpawnManager spawner;
    InitSpawnManager(&spawner);
    ALLEGRO_BITMAP* img_trash = al_load_bitmap("trash.png");
    ALLEGRO_BITMAP* img_dish = al_load_bitmap("dish.png");
    ALLEGRO_BITMAP* img_troll = al_load_bitmap("troll.png");

    // 플레이어의 위치를 설정하고 구조체를 생성하는 코드
    Player player;
    init_player(&player);

    //jgjgjgj
    bool redraw = true;
    ALLEGRO_EVENT event;

    //float player_x = 100;
    srand(time(NULL));

    //int redraw = 1;
    al_start_timer(timer);
    while (!done)
    {
        al_wait_for_event(queue, &event);

        keyboard_update(&event);
        mouse_update(&event);


        // ... 게임 루프 내부 ...
        switch (event.type) {

        case ALLEGRO_EVENT_TIMER:
            if (in_menu) {
                int result = menu_update(main_menu);
                if (result == MENU_EXIT) {
                    done = true;
                }
            }
            else {
                // 틱마다 이벤트가 생기면 플레이어의 위치를 변경하고
                // 아이템의 위치도 변경시킨다
                //UpdateSpawning(&spawner, obs_pool, MAX_OBS);
                item_update();
                item_draw();

                update_player(&player);

                item_collision_check(&game, &player);
                UpdateObstacles(obs_pool, MAX_OBS, GRAVITY, player.x);
                obstacle_collision_check(&player, obs_pool, MAX_OBS, &game);
                UpdateSpawning(&spawner, obs_pool, MAX_OBS, &game);
                if (key[ALLEGRO_KEY_ESCAPE])
                    in_menu = true;
            }

            if (key[ALLEGRO_KEY_DOWN])
                printf("Holding DOWN\n");

            redraw = true;
            frames++;
            mouse_tick();
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            if (!in_menu) {
                // 위로 가는 버튼이거나 스페이스를 누르면
                // 플레이어가 공중에 있지않으면 점프할 준비를 한다.
                if (event.keyboard.keycode == ALLEGRO_KEY_UP
                    ) {
                    if (player.state == PLAYER_RUN) {
                        player.state = PLAYER_JUMP;
                        player.jumpDirection = 1;
                        player.jumpFrame = 0;
                    }
                    printf("UP\n");
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                    if (player.state == PLAYER_RUN) {
                        player.state = PLAYER_SLIDING;
                        player.slideFrame = 0; // 프레임 초기화
                        printf("DOWN\n");
                    }
                }
            }
            break;

        case ALLEGRO_EVENT_KEY_UP:
            if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                printf("Stand\n");
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                if (player.state == PLAYER_SLIDING) {
                    player.state = PLAYER_RUN;
                    player.runFrame = 0; // 프레임 초기화
                }
            printf("Stand\n");
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;

        }


        if (done)
            break;

        keyboard_update(&event);

        if (redraw && al_is_event_queue_empty(queue))
        {
            if (in_menu) {
                menu_draw(main_menu);
            }
            else {
                // 이벤트 발생이후 bitmap에 플레이어의 x,y 좌표
                    // 아이템의 x,y 좌표를 그린 후 버퍼에 있는 bitmap을 출력한다
                    //al_clear_to_color(al_map_rgb(0, 0, 0));
                draw_map();
                draw_player(&player);

                //디버깅용
                draw_player_hitbox(&player);

                // 아이템
                item_draw();

                //장애물
                DrawObstaclesWithImage(obs_pool, MAX_OBS, img_trash, img_dish, img_troll);
                //al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            }
            al_flip_display();
            redraw = false;
        }
    }

    // 루프가 끝나면 player 구조체와 나머지 구조체들을 할당해제한다
    destroy_player(&player);
    al_destroy_timer(timer);
    al_destroy_font(menu_font);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}