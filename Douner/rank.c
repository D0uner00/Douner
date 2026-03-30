#define _CRT_SECURE_NO_WARNINGS
#include "rank.h"

int current_record_size = 0;
MENU_ITEM* rank_menu = NULL;

FILE* rank_file;
Rect box;
ALLEGRO_FONT* menu_font;

extern void on_back_to_menu();

int compare(const void* a, const void* b){
	Record* recordA = (Record*)a;
	Record* recordB = (Record*)b;

    return recordB->score - recordA->score;
}

Record* file_read() {
    rank_file = fopen("rank.txt","r");

    if (rank_file == NULL) {
        return NULL;
    }

    Record* records = (Record*)calloc(MAX_RECORD_SIZE, sizeof(Record));
    if (records == NULL) {
        fclose(rank_file);
        return NULL;
    }

    int record_count = 0;
    //int record_count = fread(records, sizeof(Record), MAX_RECORD_SIZE, rank_file);

    while (record_count < MAX_RECORD_SIZE &&
        fscanf(rank_file, "%19s %d", records[record_count].name, &records[record_count].score) == 2) {
        record_count++;
    }

    fclose(rank_file);
    rank_file = NULL;
    current_record_size = record_count;

    if (record_count > 0) {
		qsort(records, record_count, sizeof(Record), compare);
    }

    return records;
}

void file_write(Record record) {
    Record* records = file_read();

    if(records == NULL){
        records = (Record*)calloc(MAX_RECORD_SIZE, sizeof(Record));
        if (records == NULL) {
            return;
        }
		current_record_size = 0;
	}

    if (current_record_size < MAX_RECORD_SIZE) {
        records[current_record_size++] = record;
    }

    else {
        if (record.score >= records[current_record_size - 1].score) {
            records[current_record_size - 1] = record;
        }
	}

	qsort(records, current_record_size, sizeof(Record), compare);
    rank_file = fopen("rank.txt", "w");

    if (rank_file == NULL) {
        free(records);
        return;
    }
    for (int i = 0; i < current_record_size; i++) {
        fprintf(rank_file, "%s %d\n", records[i].name, records[i].score);
    }
   //fwrite(records, sizeof(Record), current_record_size, rank_file);
	free(records);
	fclose(rank_file);
}

void rank_init() {

    box.w = 400; // 박스 가로 길이
    box.h = 400; // 박스 세로 길이
    box.x = (SCREEN_WIDTH - box.w) / 2;
    box.y = (SCREEN_HEIGHT - box.h) / 2;

    if (rank_menu != NULL) {
        free(rank_menu);
        rank_menu = NULL;
    }

    int idx = 0;
    Record* records = file_read();

    rank_menu = (MENU_ITEM*)calloc(MAX_RECORD_SIZE + 5, sizeof(MENU_ITEM));
    rank_menu[idx++] = (MENU_ITEM)MENU_TEXT("RANKING");

    for (int i = 0; i < current_record_size; ++i) {
        char* buf = (char*)malloc(64);
        sprintf(buf, " %2d     %-10s %-5d", i + 1, records[i].name, records[i].score);
        rank_menu[idx++] = (MENU_ITEM)MENU_TEXT(buf);
    }

    rank_menu[idx++] = (MENU_ITEM)MENU_SPACE(20);
    rank_menu[idx++] = (MENU_ITEM)MENU_BUTTON("Back to Menu", on_back_to_menu);
    rank_menu[idx] = (MENU_ITEM)MENU_END();

    menu_init(rank_menu);
}


int rank_update() {
    return menu_update(rank_menu);
}

void rank_draw() {
    if (rank_menu == NULL) return;
	draw_box(box);
    for (int i = 0; rank_menu[i].handler != NULL; i++) {
        if (i == 0) {
            al_draw_text(menu_font, al_map_rgb(255, 255, 255), rank_menu[i].x, rank_menu[i].y, ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
            continue;
        }

        if (rank_menu[i + 1].handler == NULL) {
            al_draw_text(menu_font, al_map_rgb(255, 255, 255), rank_menu[i].x, rank_menu[i].y, ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
            continue;
        }

        int rx = box.x + 10;
        int ry = rank_menu[i].y;
        int rw = box.w - 20;    
        int rh = 30;


        al_draw_text(menu_font, al_map_rgb(255, 255, 255), rank_menu[i].x, ry + 10, ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
    }

}

// 임시 //
void processRank() {
    Record initial_records[5] = {
        {"Hero_A", 5000, 3},
        {"Player_B", 3500, 2},
        {"Lucky_C", 2000, 1},
        {"Beginner", 500, 1},
        {"Pro_Gamer", 8000, 3}
    };

    rank_file = fopen("rank.dat", "wb");
    if (rank_file != NULL) {
		qsort(initial_records, 5, sizeof(Record), compare);
        fwrite(initial_records, sizeof(Record), 5, rank_file);
        fclose(rank_file);
        rank_file = NULL;
    }

    Record* loaded_records = file_read(5);

    if (loaded_records != NULL) {

        for (int i = 0; i < current_record_size; i++) {
            printf("%s\t%d\t%d\n",
                loaded_records[i].name,
                loaded_records[i].score,
                loaded_records[i].difficulty);
        }
        free(loaded_records);
    }
    else {
        exit(1);
    }
}
