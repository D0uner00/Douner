#define _CRT_SECURE_NO_WARNINGS
#include "rank.h"

int current_record_size = 0;
static char rank_buffer[MAX_RECORD_SIZE][64];

MENU_ITEM* rank_menu = NULL;

FILE* rank_file;
Rect box;
ALLEGRO_FONT* menu_font;
ALLEGRO_BITMAP* img_rank_board = NULL;
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
  fclose(rank_file);
}

void rank_init() {
    if (!img_rank_board) img_rank_board = al_load_bitmap("rank_board.png");
    if (!background) background = al_load_bitmap("background\\background_rank.png");

    if (img_rank_board) {
        box.w = al_get_bitmap_width(img_rank_board);
        box.h = al_get_bitmap_height(img_rank_board);
    }
    else {
        box.w = 400; box.h = 500; // 이미지 없을 때 예비용
    }
    box.x = (SCREEN_WIDTH - box.w) / 2;
    box.y = (SCREEN_HEIGHT - box.h) / 2;

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
    
    al_draw_bitmap(background, 0, 0, 0);

    if (img_rank_board) {
        al_draw_bitmap(img_rank_board, box.x, box.y, 0);
    }
    else {
        draw_box(box);
    }

    int start_y = box.y + 55;  // 'TOP RANKS' 간판 바로 아래부터 시작 (필요시 조절)
    int line_height = 25;      // 나무 판자 한 칸의 대략적인 높이 (필요시 조절)
    int text_center_x = box.x + (box.w / 2)+ 15; // 글씨는 무조건 이미지 한가운데

    for (int i = 0; rank_menu[i].handler != NULL; i++) {

        //Back to Menu
        if (rank_menu[i + 1].handler == NULL) {
            al_draw_text(menu_font, al_map_rgb(255, 255, 255),
                text_center_x, box.y + box.h - 15, // 게시판 맨 밑바닥
                ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
            continue;
        }

        // 랭킹 기록(1~10등)이 그려질 Y 좌표 계산
        int current_y = start_y + (i * line_height);

        // 1, 2, 3등 색깔 구별
        ALLEGRO_COLOR text_color = al_map_rgb(255, 255, 255); // 기본
        if (i == 0) text_color = al_map_rgb(255, 215, 0);       // 1등 금
        else if (i == 1) text_color = al_map_rgb(192, 192, 192); // 2등 은
        else if (i == 2) text_color = al_map_rgb(205, 127, 50);  // 3등 동

        al_draw_text(menu_font, text_color,
            text_center_x, current_y,
            ALLEGRO_ALIGN_CENTER, rank_menu[i].text);
    }
}

// 게임 종료 시 Main에서 호출하여 이미지 메모리 해제
void rank_deinit() {
    if (img_rank_board) {
        al_destroy_bitmap(img_rank_board);
        img_rank_board = NULL;
    }

}