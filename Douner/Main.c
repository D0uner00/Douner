#include "global.h"
#include "keyboard.h"
#include "mouse.h"
#include "menu.h"
#include "item.h"
#include "Player.h"
//#include "obstacle.h"

ALLEGRO_FONT* menu_font;

long frames;
long score = 0;
bool done = false;

GameScreen cur_screen = SCREEN_MENU;

void on_start() {
    cur_screen = SCREEN_PLAY; // 게임 시작
}

void on_exit() {
    done = true;    // 프로그램 종료
}

void on_enter_name() {
    cur_screen = SCREEN_NAME_INPUT;
}

MENU_ITEM main_menu[] = {
    MENU_BUTTON("Enter Name",on_enter_name),
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

    // 플레이어의 위치를 설정하고 구조체를 생성하는 코드
    Player player;
    init_player(&player);

    bool redraw = true;
    ALLEGRO_EVENT event;

    srand(time(NULL));

    al_start_timer(timer);
    while (!done)
    {
        al_wait_for_event(queue, &event);

        keyboard_update(&event);
        mouse_update(&event);

        switch (event.type) {

        case ALLEGRO_EVENT_TIMER:

            switch (cur_screen) {
            case SCREEN_MENU:
                menu_update(main_menu);
                break;

            case SCREEN_PLAY:
                item_update();

                update_player(&player);

                item_collision_check(&game, &player);

                if (key[ALLEGRO_KEY_ESCAPE])
                    cur_screen = SCREEN_MENU;

            case SCREEN_NAME_INPUT:
                break;
            }
            
            redraw = true;
            frames++;
            mouse_tick();
            break;

        case ALLEGRO_EVENT_KEY_DOWN:

            switch(cur_screen){
            case SCREEN_PLAY:
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
                break;

            case SCREEN_NAME_INPUT:
                break;
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
                switch (cur_screen) {
                case SCREEN_MENU:
                    menu_draw(main_menu);
                    break;

                case SCREEN_PLAY:
                    draw_map();
                    draw_player(&player);
                    //debug
                    draw_player_hitbox(&player);
                    item_draw();
                    break;

                case SCREEN_NAME_INPUT:
                    break;
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