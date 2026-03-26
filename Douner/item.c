#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "item.h"
#include "global.h"
#include "Player.h"

Item items[ITEM_MAX];
extern long frames;


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
                items[i].x = SCREEN_WIDTH + 200;
                items[i].y = 100 + rand() % 150;
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

            //디버깅용
            // 히트박스 표시 (빨간색)
            al_draw_rectangle(
                items[i].x,
                items[i].y,
                items[i].x + 10,
                items[i].y + 10,
                al_map_rgb(255, 0, 0),
                1
            );
        }
    }
}


void item_collision_check(GameState* game, Player* player)
{
    Rect pBox = get_player_hitbox(player);

    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (!items[i].active) continue;
        
        Rect iBox = {
            items[i].x,
            items[i].y,
            20,
            20
        };

        if (collide_rect(pBox, iBox))
        {
            items[i].active = 0;
            game->score += 10;

            printf("ITEM GET! score = %d\n", game->score);
        }
        
    }
}