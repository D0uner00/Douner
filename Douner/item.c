#include "item.h"
#include "global.h"
#include "Player.h"
#include "hitbox.h"

static ALLEGRO_BITMAP* sheet_star = NULL;
static ALLEGRO_BITMAP* sheet_hp = NULL;

Item items[ITEM_MAX];
extern long frames;

void item_init()
{
    sheet_star = al_load_bitmap("star.png");
    if (!sheet_star) {
        printf("star.png 로드 실패!\n");
        return;
    }

    sheet_hp = al_load_bitmap("hp.png");
    if (!sheet_hp) {
        printf("hp.png 로드 실패!\n");
        return;
    }

    for (int i = 0; i < ITEM_MAX; i++)
    {
        items[i].active = 0;
        items[i].frame = 0;
        items[i].sheet = NULL;
        items[i].x = 0;
        items[i].y = 0;
        items[i].hpAmount = 0;
    }
}

void item_update()
{
    // 스타: 2초마다 (120프레임), HP: 가끔 (300프레임마다)
    int spawn_star = (frames % STAR_SPAWN_INTERVAL == 0);
    int spawn_hp = (frames % HP_SPAWN_INTERVAL == 0 && frames > 0);

    if (spawn_star || spawn_hp)
    {
        for (int i = 0; i < ITEM_MAX; i++)
        {
            if (!items[i].active)
            {
                if (spawn_hp) {
                    items[i].type = ITEM_HP;
                    items[i].sheet = sheet_hp;
                    items[i].maxFrame = 4;
                    items[i].width = al_get_bitmap_width(sheet_hp)/4;
                    items[i].height = al_get_bitmap_height(sheet_hp);
                    items[i].hpAmount = 20;
                }
                else {
                    items[i].type = ITEM_STAR;
                    items[i].sheet = sheet_star;
                    items[i].maxFrame = 4;
                    items[i].width = al_get_bitmap_width(sheet_star) / 4;
                    items[i].height = al_get_bitmap_height(sheet_star);
                    items[i].hpAmount = 0;
                }

                items[i].x = SCREEN_WIDTH + 200;
                items[i].y = 100 + rand() % 150;
                items[i].active = 1;
                items[i].frame = 0;
                break;
            }
        }
    }

    // 이동 & 애니메이션
    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (!items[i].active) continue;

        items[i].x -= ITEM_SPEED;

        if (frames % ANIM_INTERVAL == 0)
            items[i].frame = (items[i].frame + 1) % items[i].maxFrame;

        if (items[i].x < -items[i].width)
            items[i].active = 0;
    }
}

void item_draw()
{
    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (!items[i].active) continue;

        float scale = (items[i].type == ITEM_HP) ? HP_SCALE : STAR_SCALE;
        int   frameX = items[i].frame * items[i].width;

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
    Rect  pBox = get_player_hitbox(player);

    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (!items[i].active) continue;

        float scale = (items[i].type == ITEM_HP) ? HP_SCALE : STAR_SCALE;

        Rect iBox = {
            items[i].x,
            items[i].y,
            items[i].width * scale,
            items[i].height * scale
        };

        if (collide_rect(pBox, iBox))
        {
            items[i].active = 0;

            if (items[i].type == ITEM_STAR)
            {
                game->score += 10;
                printf("STAR GET! score = %d\n", game->score);
            }
            else if (items[i].type == ITEM_HP)
            {
                game->hp += items[i].hpAmount;
                if (game->hp > 100.0f) game->hp = 100.0f;
                printf("HP GET! hp = %.1f\n", game->hp);
            }
        }
    }
}

void item_deinit()
{
    if (sheet_star) {
        al_destroy_bitmap(sheet_star);
        sheet_star = NULL;
    }
    if (sheet_hp) {
        al_destroy_bitmap(sheet_hp);
        sheet_hp = NULL;
    }
    for (int i = 0; i < ITEM_MAX; i++)
        items[i].sheet = NULL;
}