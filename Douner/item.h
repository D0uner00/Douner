#ifndef ITEM_H
#define ITEM_H

typedef struct {
    int x, y;
    int active;
} Item;

#define ITEM_MAX 10
Item items[ITEM_MAX];

void item_init();
void item_update();
void item_draw();
int collide(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh);
void item_collision_check();

#endif