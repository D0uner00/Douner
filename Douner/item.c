#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "item.h"
#include "global.h"

Item items[ITEM_MAX];
extern long frames;

extern int player_x;
extern int player_y;
extern int player_w;
extern int player_h;

void item_init()
{
    for (int i = 0; i < ITEM_MAX; i++)
        items[i].active = 0;
}

void item_update()
{
    // 생성 (2초마다)
    if (frames % 60 == 0)
    {
        for (int i = 0; i < ITEM_MAX; i++)
        {
            if (!items[i].active)
            {
                items[i].x = 320;
                items[i].y = 100;
                items[i].active = 1;
                break;
            }
        }
    }

    // 이동
    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (items[i].active)
        {
            items[i].x -= 3;

            if (items[i].x < 0)
                items[i].active = 0;
        }
    }
}

void item_draw()
{
    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (items[i].active)
        {
            al_draw_filled_rectangle(
                items[i].x,
                items[i].y,
                items[i].x + 10,
                items[i].y + 10,
                al_map_rgb(255, 255, 0)
            );
        }
    }
}


void item_collision_check()
{
    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (items[i].active)
        {
            if (collide(player_x, player_y, player_w, player_h,
                items[i].x, items[i].y, 10, 10))
            {
                printf("ITEM GET!\n");
                items[i].active = 0;
            }
        }
    }
}