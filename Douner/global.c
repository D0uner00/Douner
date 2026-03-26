#include "global.h"

void must_init(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int collide(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
{
    return !(ax > bx + bw || ax + aw < bx ||
        ay > by + bh || ay + ah < by);
}

void game_init(GameState* game)
{
    game->score = 0;
    game->life = 3;
    game->difficulty = 0;
}
