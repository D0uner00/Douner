#ifndef _RANK_H_
#define _RANK_H_
#include "global.h"

typedef struct record{
	char name[20];
	int score;
	int difficulty;
}Record;

//int exists_in_ranking(const char* name);
void file_read(Record* records, int count);
void file_write(Record* records, int count);

#endif // !_RANK_H_
