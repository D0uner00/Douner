#ifndef ITEM_H
#define ITEM_H

#include "global.h"
#include "Player.h"
#define STAR_SPAWN_INTERVAL 120
#define HP_SPAWN_INTERVAL   300
#define ITEM_SPEED          3
#define ANIM_INTERVAL       10
#define STAR_SCALE 0.4f
#define HP_SCALE   0.3f

typedef enum {
    ITEM_STAR,
    ITEM_HP
} ItemType;

typedef struct {
    int x, y;
    int active;

    ALLEGRO_BITMAP* sheet;

    int frame;
    int maxFrame;

    int width;
    int height;

    ItemType type;
    int hpAmount;
} Item;

#define ITEM_MAX 10
extern Item items[ITEM_MAX];

void item_init();
void item_update();
void item_draw();
void item_collision_check(GameState* game, Player* player);
void item_deinit();

#endif