#ifndef ITEM_H
#define ITEM_H
#include "global.h"
#include "Player.h"

typedef struct {
    int x, y;
    int active;
} Item;

#define ITEM_MAX 10
extern Item items[ITEM_MAX];

void item_init();
void item_update();
void item_draw();
void item_collision_check(GameState* game, Player* player);

#endif