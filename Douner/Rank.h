#ifndef _RANK_H_
#define _RANK_H_
#include "global.h"

typedef struct record{
	char name[20];
	int score;
	int difficulty;
}Record;


void file_read(Record* records, int count);
void file_write(Record* records, int count);

#endif // !_RANK_H_
