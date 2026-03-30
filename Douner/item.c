#include "item.h"
#include "global.h"
#include "Player.h"
#include "hitbox.h"

static ALLEGRO_BITMAP* sheet = NULL;
Item items[ITEM_MAX];
extern long frames;


void item_init()
{
    ALLEGRO_BITMAP* sheet = al_load_bitmap("star.png");

    if (!sheet) {
        printf("아이템 이미지 로드 실패!\n");
        return;
    }


    int maxFrame = 4;
    int frameW = al_get_bitmap_width(sheet) / maxFrame;
    int frameH = al_get_bitmap_height(sheet);

    printf("star frame size: %d x %d\n", frameW, frameH);

    for (int i = 0; i < ITEM_MAX; i++)
    {
        items[i].sheet = sheet;   // all share one bitmap
        items[i].x = 0;
        items[i].y = 0;
        items[i].active = 0;
        items[i].frame = 0;
        items[i].maxFrame = maxFrame;
        items[i].width = frameW;
        items[i].height = frameH;
    }
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
                items[i].frame = 0;
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

            if (frames % 10 == 0)
            {
                items[i].frame = (items[i].frame + 1) % items[i].maxFrame;
            }

            if (items[i].x < 0)
                items[i].active = 0;
        }
    }
}

void item_draw()
{
    
    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (!items[i].active) continue;

        int frameX = items[i].frame * items[i].width;
        float scale = 0.4f;
        al_draw_scaled_bitmap(
            items[i].sheet,
            frameX, 0,
            items[i].width,
            items[i].height,
            items[i].x,
            items[i].y,
            items[i].width * scale,
            items[i].height * scale,
            0
        );
    }
}

void item_collision_check(GameState* game, Player* player)
{
    Rect pBox = get_player_hitbox(player);
    float scale = 0.4f;

    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (!items[i].active) continue;
        
        Rect iBox = {
            items[i].x,
            items[i].y,
            items[i].width*scale,
            items[i].height*scale
        };

        if (collide_rect(pBox, iBox))
        {
            items[i].active = 0;
            game->score += 10;

            printf("ITEM GET! score = %d\n", game->score);
        }
        
    }
}

void item_deinit()
{
    if (items[0].sheet)
    {
        al_destroy_bitmap(items[0].sheet);
        for (int i = 0; i < ITEM_MAX; i++)
            items[i].sheet = NULL;
    }
}