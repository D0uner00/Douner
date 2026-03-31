#ifndef ITEM_H
#define ITEM_H

#include "global.h"
#include "Player.h"
#include "hitbox.h"

#define ITEM_MAX 10
#define ITEM_SPEED 3
#define ANIM_INTERVAL 10

// Forward declaration of Item for the function pointer
typedef struct Item Item;

typedef void (*ItemEffect)(Item* item, GameState* game, Player* player);

typedef enum {
    ITEM_STAR,
    ITEM_HP,
    ITEM_MAX_TYPE
} ItemTypes;

typedef struct {
    const char* filename;
    ALLEGRO_BITMAP* sheet;
    int maxFrame;
    float scale;
    int hpAmount;
    int scoreAmount;
    int spawnInterval;
    ItemEffect effect;
} ItemTypeData;

struct Item {
    float x, y; // Use float for smoother movement
    int active;
    int frame;
    int width;
    int height;
    ItemTypes type;
};

// Interface
void item_init();
void item_update(GameState* game, Player* player);
void item_draw();
void item_deinit();
void spawn_item(ItemTypes type);

void draw_item_hitboxes();

#endif