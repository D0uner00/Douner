#include "global.h"
#include "keyboard.h"
#include "mouse.h"
#include "menu.h"
#include "hud.h"
#include "item.h"
#include "Player.h"
//#include "obstacle.h"

ALLEGRO_FONT* menu_font;

long frames;
long score = 0;
bool done = false;
bool in_menu = true;

void on_start() {
    in_menu = false;
}

void on_exit() {
    done = true;
}

MENU_ITEM main_menu[] = {
    MENU_BUTTON("TEMP", NULL),
    MENU_BUTTON("Start Game", on_start),
    MENU_BUTTON("Exit", on_exit),
    MENU_BUTTON("TEMP", NULL),
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
    hud_init(); // [수정] HUD 전용 큰 폰트 초기화

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    keyboard_init();
    item_init();
    menu_init(main_menu);

    GameState game;
    game_init(&game);

    Player player;
    init_player(&player);

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
            if (in_menu) {
                if (menu_update(main_menu) == MENU_EXIT) done = true;
            }
            else {
                item_update();
                update_player(&player);
                item_collision_check(&game, &player);
                if (key[ALLEGRO_KEY_ESCAPE]) in_menu = true;
            }
            redraw = true;
            frames++;
            mouse_tick();
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            if (!in_menu) {
                if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                    if (player.state == PLAYER_RUN) {
                        player.state = PLAYER_JUMP;
                        player.jumpDirection = 1;
                        player.jumpFrame = 0;
                    }
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                    if (player.state == PLAYER_RUN) {
                        player.state = PLAYER_SLIDING;
                        player.slideFrame = 0;
                    }
                }
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
        }

        if (done) break;

        // [수정] 그리기 로직 통합 및 중괄호 정리
        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0)); // 배경 초기화

            if (in_menu) {
                menu_draw(main_menu);
            }
            else {
                draw_map();
                draw_player(&player);
                draw_player_hitbox(&player);
                item_draw();
                hud_draw(&game); // [수정] 점수 출력
            }

            al_flip_display();
            redraw = false;
        }
    }

    // [수정] 해제 코드 위치 및 HUD 해제 추가
    destroy_player(&player);
    hud_deinit();
    al_destroy_timer(timer);
    al_destroy_font(menu_font);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}