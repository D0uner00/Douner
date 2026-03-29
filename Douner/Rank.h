#ifndef _RANK_H_
#define _RANK_H_
#include "global.h"
#include "menu.h"

#define MAX_RECORD_SIZE 10

extern int current_record_size;
extern MENU_ITEM* rank_menu;

typedef struct record{
	char name[20];
	int score;
	int difficulty;
}Record;

Record* file_read();
void file_write(Record record);

void rank_init();
int rank_update();
void rank_draw();

//int exists_in_ranking(const char* name);

void processRank();
#endif // !_RANK_H_
