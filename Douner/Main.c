#include "global.h"
#include "keyboard.h"
#include "mouse.h"
#include "menu.h"
#include "hud.h"
#include "item.h"
#include "Player.h"
#include "obstacle.h"
#include "Background.h"

ALLEGRO_FONT* menu_font;
long frames;
long score = 0;
bool done = false;
bool in_menu = true;

void on_start() { in_menu = false; }
void on_exit() { done = true; }

MENU_ITEM main_menu[] = {
    MENU_BUTTON("TEMP", NULL),
    MENU_BUTTON("Start Game", on_start),
    MENU_BUTTON("Ranking", NULL),
    MENU_BUTTON("Exit", on_exit),
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
    hud_init();

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    keyboard_init();
    item_init();
    menu_init(main_menu);

    GameState game;
    game_init(&game); // game.hp = 100, game.score = 0 초기화 포함 

    Obstacle obs_pool[MAX_OBS];
    InitObstacles(obs_pool, MAX_OBS);
    SpawnManager spawner;
    InitSpawnManager(&spawner);

    ALLEGRO_BITMAP* img_trash = al_load_bitmap("trash.png");
    ALLEGRO_BITMAP* img_dish = al_load_bitmap("dish.png");
    ALLEGRO_BITMAP* img_troll = al_load_bitmap("troll.png");

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
            if (in_menu) {
                if (menu_update(main_menu) == MENU_EXIT) done = true;
            }
            else {
                update_background(&bg);
                item_update();
                update_player(&player);
                item_collision_check(&game, &player);
                UpdateObstacles(obs_pool, MAX_OBS, GRAVITY, player.x);
                obstacle_collision_check(&player, obs_pool, MAX_OBS, &game);
                UpdateSpawning(&spawner, obs_pool, MAX_OBS, &game);
                hud_update(&game); // HP 서서히 감소 로직 

                if (key[ALLEGRO_KEY_ESCAPE]) in_menu = true;
                if (game.hp <= 0) done = true; // 사망 시 종료 (또는 게임오버 처리) 
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

        // --- 그리기 로직 (루프 내부로 위치 수정) ---
        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0)); // 화면 초기화 필수 

            if (in_menu) {
                menu_draw(main_menu);
            }
            else {
                draw_background(&bg);
                draw_player(&player);
                draw_player_hitbox(&player);
                item_draw();
                DrawObstaclesWithImage(obs_pool, MAX_OBS, img_trash, img_dish, img_troll);
                hud_draw(&game); // HP바와 점수 출력 
            }
            al_flip_display();
            redraw = false;
        }
    }

    // --- 정리 및 자원 해제 ---
    destroy_player(&player);
    hud_deinit();
    al_destroy_timer(timer);
    al_destroy_font(menu_font);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    al_destroy_bitmap(img_trash);
    al_destroy_bitmap(img_dish);
    al_destroy_bitmap(img_troll);

    return 0;
}