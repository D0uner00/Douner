#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "keyboard.h"
#include "hud.h"
#include "item.h"

long frames;
long score;

int player_x = 50;
int player_y = 100;
int player_w = 20;
int player_h = 20;

int main()
{
    al_init();
    al_install_keyboard();


    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();


    ALLEGRO_DISPLAY* display = al_create_display(320, 200);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    keyboard_init();
    item_init();

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type) {

        case ALLEGRO_EVENT_KEY_DOWN:
            if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                printf("UP\n");
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                printf("DOWN\n");
            break;

        case ALLEGRO_EVENT_KEY_UP:
            if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                printf("Stand\n");
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                printf("Stand\n");
            break;

        case ALLEGRO_EVENT_TIMER:

            item_update();
            item_collision_check();

            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            if (key[ALLEGRO_KEY_DOWN])
                printf("Holding DOWN\n");

            redraw = true;
            frames++;
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
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // 플레이어 대신 테스트 박스
            al_draw_filled_rectangle(player_x, player_y,
                player_x + player_w,
                player_y + player_h,
                al_map_rgb(0, 255, 0));

            // 아이템
            item_draw();

            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}