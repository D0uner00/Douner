#include "item.h"

// 1. Data-Driven Configuration: Everything about an item is defined here.
// To add a new item, just add an entry to this table and an enum value.
ItemTypeData itemTypes[ITEM_MAX_TYPE] = {
    [ITEM_STAR] = { "star.png", NULL, 4, 0.4f, 0, 10, 120, NULL },
    [ITEM_HP] = { "hp.png",   NULL, 4, 0.3f, 20, 0, 300, NULL }
};

Item items[ITEM_MAX];
extern long frames;

// Private Effect Functions
static void effect_star(Item* item, GameState* game, Player* player) {
    game->score += itemTypes[item->type].scoreAmount;
}

static void effect_hp(Item* item, GameState* game, Player* player) {
    game->hp += itemTypes[item->type].hpAmount;
    if (game->hp > 100.0f) game->hp = 100.0f;
}

void item_init() {
    // Assign effects here (or do it directly in the global array if functions are declared)
    itemTypes[ITEM_STAR].effect = effect_star;
    itemTypes[ITEM_HP].effect = effect_hp;

    for (int i = 0; i < ITEM_MAX_TYPE; i++) {
        itemTypes[i].sheet = al_load_bitmap(itemTypes[i].filename);
        if (!itemTypes[i].sheet) {
            fprintf(stderr, "Failed to load: %s\n", itemTypes[i].filename);
        }
    }

    for (int i = 0; i < ITEM_MAX; i++) {
        items[i].active = 0;
    }
}

void spawn_item(ItemTypes type) {
    if (type < 0 || type >= ITEM_MAX_TYPE) return;

    for (int i = 0; i < ITEM_MAX; i++) {
        if (!items[i].active) {
            ItemTypeData* data = &itemTypes[type];

            items[i].active = 1;
            items[i].type = type;
            items[i].frame = 0;
            items[i].width = al_get_bitmap_width(data->sheet) / data->maxFrame;
            items[i].height = al_get_bitmap_height(data->sheet);
            items[i].x = SCREEN_WIDTH + 100;
            items[i].y = 100 + rand() % 250;
            return;
        }
    }
}

void item_collision_check(GameState* game, Player* player) {
    Rect pBox = get_player_hitbox(player);

    for (int i = 0; i < ITEM_MAX; i++) {
        if (!items[i].active) continue;

        ItemTypeData* data = &itemTypes[items[i].type];
        Rect iBox = {
            items[i].x, items[i].y,
            items[i].width * data->scale, items[i].height * data->scale
        };

        if (collide_rect(pBox, iBox)) {
            items[i].active = 0;
            if (data->effect) data->effect(&items[i], game, player);
        }
    }
}

void item_update(GameState* game, Player* player) {
    // Automatic Spawning based on data
    for (int i = 0; i < ITEM_MAX_TYPE; i++) {
        if (frames > 0 && frames % itemTypes[i].spawnInterval == 0) {
            spawn_item((ItemTypes)i);
        }
    }

    // Movement and Animation
    for (int i = 0; i < ITEM_MAX; i++) {
        if (!items[i].active) continue;

        items[i].x -= ITEM_SPEED;

        if (frames % ANIM_INTERVAL == 0) {
            items[i].frame = (items[i].frame + 1) % itemTypes[items[i].type].maxFrame;
        }

        if (items[i].x < -100) items[i].active = 0;
    }

    item_collision_check(game, player);
}

void item_draw() {
    for (int i = 0; i < ITEM_MAX; i++) {
        if (!items[i].active) continue;

        ItemTypeData* data = &itemTypes[items[i].type];
        int frameX = items[i].frame * items[i].width;

        al_draw_scaled_bitmap(
            data->sheet,
            frameX, 0, items[i].width, items[i].height,
            items[i].x, items[i].y,
            items[i].width * data->scale, items[i].height * data->scale,
            0
        );
    }
}

void item_deinit() {
    for (int i = 0; i < ITEM_MAX_TYPE; i++) {
        if (itemTypes[i].sheet) {
            al_destroy_bitmap(itemTypes[i].sheet);
            itemTypes[i].sheet = NULL;
        }
    }
}