#define _CRT_SECURE_NO_WARNINGS
#include "rank.h"

int current_record_size = 0;
static char rank_buffer[MAX_RECORD_SIZE][64];

MENU_ITEM* rank_menu = NULL;

FILE* rank_file;
Rect box;
ALLEGRO_FONT* menu_font;
ALLEGRO_BITMAP* background = NULL;

extern void on_back_to_menu();

int compare(const void* a, const void* b) {
    Record* recordA = (Record*)a;
    Record* recordB = (Record*)b;

    return recordB->score - recordA->score;
}

Record* file_read() {
    rank_file = fopen("rank.txt", "r");

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

    if (records == NULL) {
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
        else {
            free(records);
            return;
        }
	}

    qsort(records, current_record_size, sizeof(Record), compare);
    rank_file = fopen("rank.txt", "w");

    if (rank_file != NULL) {
        for (int i = 0; i < current_record_size; i++) {
            fprintf(rank_file, "%s %d\n", records[i].name, records[i].score);
        }
        fclose(rank_file);
    }
	free(records);
}

void rank_init() {
    if (!background) background = al_load_bitmap("background\\background_rank_.png");


    if (rank_menu != NULL) {
        free(rank_menu);
        rank_menu = NULL;
    }

    int idx = 0;
    Record* records = file_read();

    rank_menu = (MENU_ITEM*)calloc(MAX_RECORD_SIZE + 5, sizeof(MENU_ITEM));

    for (int i = 0; i < current_record_size; ++i) {
        sprintf(rank_buffer[i], " %2d     %-10s %-5d", i + 1, records[i].name, records[i].score);
        rank_menu[idx++] = (MENU_ITEM)MENU_TEXT(rank_buffer[i]);
    }

    rank_menu[idx++] = (MENU_ITEM)MENU_SPACE(10);
    rank_menu[idx++] = (MENU_ITEM)MENU_BUTTON("Back to Menu", on_back_to_menu);
    rank_menu[idx] = (MENU_ITEM)MENU_END();

    menu_init(rank_menu);
	free(records);
}


int rank_update() {
    return menu_update(rank_menu);
}

void rank_draw() {
    if (rank_menu == NULL) return;

    al_draw_bitmap(background, 0, 0, 0);
    //draw_box(box);

    int text_center_x = box.x + (box.w / 2); // 글씨는 무조건 이미지 한가운데

    for (int i = 0; rank_menu[i].handler != NULL; i++) {
        int draw_y = rank_menu[i].y;

        //Back to Menu
        if (rank_menu[i].handler == menu_button_handler) {
            al_draw_text(menu_font, al_map_rgb(255, 255, 255),
                text_center_x, draw_y,
                ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
            continue;
        }

        // 1, 2, 3등 색깔 구별
        ALLEGRO_COLOR text_color = al_map_rgb(255, 255, 255); // 기본
        if (i == 0) text_color = al_map_rgb(255, 215, 0);       // 1등 금
        else if (i == 1) text_color = al_map_rgb(192, 192, 192); // 2등 은
        else if (i == 2) text_color = al_map_rgb(205, 127, 50);  // 3등 동

        al_draw_text(menu_font, text_color,
            text_center_x, draw_y,
            ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
    }
}

// 게임 종료 시 Main에서 호출하여 이미지 메모리 해제
void rank_deinit() {
    al_destroy_bitmap(background);
    background = NULL;
}