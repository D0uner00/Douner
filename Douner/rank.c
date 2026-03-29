#define _CRT_SECURE_NO_WARNINGS
#include "rank.h"


int current_record_size = 0;
MENU_ITEM* rank_menu = NULL;

FILE* rank_file;
Rect box;
ALLEGRO_FONT* menu_font;

Record* file_read() {
    rank_file = fopen("rank.dat", "rb");

    if (rank_file == NULL) {
        return NULL;
    }

    Record* records = (Record*)malloc(sizeof(Record) * MAX_RECORD_SIZE);
    if (records == NULL) {
        fclose(rank_file);
        return NULL;
    }

    int record_count = fread(records, sizeof(Record), MAX_RECORD_SIZE, rank_file);

    fclose(rank_file);
    rank_file = NULL;
    current_record_size = record_count;
    return records;
}

void file_write(Record record) {
	// 임시용 제일 점수가 낮은걸 찾아서 교체하게 구현 예정 //
	if (current_record_size >= MAX_RECORD_SIZE) {

		return;
	}
	/////////////////////////////////////////////////////////
	// 정렬도 구현해야함 //
	rank_file = fopen("rank.dat", "ab");

	if (rank_file == NULL) {
		return;
	}

	fwrite(&record, sizeof(Record), 1, rank_file);
	current_record_size++;
	fclose(rank_file);
	rank_file = NULL;
}

void rank_init() {
    if (rank_menu == NULL) {
        int idx = 0;
        Record* records = file_read(MAX_RECORD_SIZE);

        rank_menu = (MENU_ITEM*)malloc(sizeof(MENU_ITEM) * (MAX_RECORD_SIZE + 3));
		rank_menu[idx++] = (MENU_ITEM)MENU_TEXT("RANKING");

        for (int i = 0;i < current_record_size;++i) {
			char* buf = (char*)malloc(64);
            sprintf(buf, "%d   %s   %d   %d", i + 1, records[i].name, records[i].score, records[i].difficulty);
			rank_menu[idx++] = (MENU_ITEM)MENU_TEXT(buf);
        }

		// 버튼으로 바꿔야됨 //
		rank_menu[idx++] = (MENU_ITEM)MENU_TEXT("Back to Menu");
        ///////////////////////
        rank_menu[idx] = (MENU_ITEM)MENU_END();

		menu_init(rank_menu);
    }
}

int rank_update() {
    return menu_update(rank_menu);
}

void rank_draw() {
    if (rank_menu == NULL) return;

    al_draw_rectangle(box.x, box.y, box.x + box.w, box.y + box.h, al_map_rgb(255, 255, 255), 2);

    for (int i = 0; rank_menu[i].handler != NULL; i++) {
        if (i == 0) {
            al_draw_text(menu_font, al_map_rgb(255, 255, 255), rank_menu[i].x, rank_menu[i].y, ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
            continue;
        }

        if (rank_menu[i + 1].handler == NULL) {
            al_draw_rectangle(rank_menu[i].x - 40, rank_menu[i].y, rank_menu[i].x + 40, rank_menu[i].y + 25, al_map_rgb(255, 255, 255), 1);
            al_draw_text(menu_font, al_map_rgb(255, 255, 255), rank_menu[i].x, rank_menu[i].y + 5, ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
            continue;
        }

        int rx = box.x + 10;
        int ry = rank_menu[i].y;
        int rw = box.w - 20;    
        int rh = 30;            

        al_draw_rectangle(rx, ry, rx + rw, ry + rh, al_map_rgb(255, 255, 255), 1);

        al_draw_line(rx + 40, ry, rx + 40, ry + rh, al_map_rgb(255, 255, 255), 1);

        al_draw_line(rx + rw - 80, ry, rx + rw - 80, ry + rh, al_map_rgb(255, 255, 255), 1);

        al_draw_text(menu_font, al_map_rgb(255, 255, 255), rank_menu[i].x, ry + 8, ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
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
