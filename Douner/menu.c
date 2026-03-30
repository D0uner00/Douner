#include "menu.h"
#include "ui.h"

static int item_count = 0;
static int selected_item = 0;
extern Rect box;

extern ALLEGRO_FONT* menu_font;

void menu_init(MENU_ITEM* menu)
{
	int i = 0;
	item_count = 0;

	// 메뉴 항목 초기화
	for (i = 0; menu[i].handler != NULL; i++)
	{
		menu[i].handler(&menu[i], MSG_INIT, 0);
	}

	item_count = i;

	// 첫번째 선택 가능한 항목을 선택 상태로 설정
	for (i = 0; i < item_count;++i) {

		if (menu[i].flags & FLAG_SELECTABLE) {
			selected_item = i;
			menu[i].flags |= FLAG_SELECTED;
			break;
		}
	}

	int max_text_width = 0;
	int total_height = 0;
	int line_spacing = 15;
	int padding = 30;     

	for (i = 0; i < item_count; i++) {

		int w = al_get_text_width(menu_font, menu[i].text);
		int h = al_get_font_line_height(menu_font);

		if (w > max_text_width) max_text_width = w;

		total_height += h + line_spacing;
	}

	total_height -= line_spacing;

	box = make_center_rect(
		max_text_width + (padding * 2),
		total_height + (padding * 2)
	);

	int current_y = box.y + padding;

	for (i = 0; i < item_count; i++) {
		menu[i].x = box.x + box.w / 2;
		menu[i].y = current_y;
		current_y += menu[i].h + line_spacing;
	}
}

int menu_update(MENU_ITEM* menu)
{
	int i, tmp;
	
	//draw_map();

	// 프레임 업데이트
	for (i = 0;i < item_count;++i) {
		if (menu[i].handler) {
			menu[i].handler(&menu[i], MSG_TICK, 0);
		}
	}

	if (key_pressed(ALLEGRO_KEY_ESCAPE)) {
		return MENU_BACK;
	}

	if(mouse_button_pressed(1)){

		int mx = mouse_x();
		int my = mouse_y();

		for(i = 0;i<item_count;++i){

			if (menu[i].flags & FLAG_SELECTABLE) {

				if (mx >= menu[i].x - (menu[i].w / 2) && mx <= menu[i].x + menu[i].w &&
					my >= menu[i].y  && my <= menu[i].y + menu[i].h) {
					if (selected_item != -1 && selected_item != i) {
						menu[selected_item].flags &= ~FLAG_SELECTED;
					}

					selected_item = i;
					menu[i].flags |= FLAG_SELECTED;

					return menu[i].handler(&menu[i], MSG_INPUT, 0);
				}
			}
		}
	}

	if (key_pressed(ALLEGRO_KEY_UP)) {
		if (selected_item != -1) {

			tmp = selected_item;

			while (1) {
				tmp = (tmp - 1 + item_count) % item_count;

				if (menu[tmp].flags & FLAG_SELECTABLE) {
					menu[selected_item].flags &= ~FLAG_SELECTED;
					selected_item = tmp;
					menu[selected_item].flags |= FLAG_SELECTED;
					break;
				}
			}
		}
	}

	if (key_pressed(ALLEGRO_KEY_DOWN)) {
		if (selected_item != -1) {

			tmp = selected_item;

			while (1) {
				tmp = (tmp + 1) % item_count;

				if (menu[tmp].flags & FLAG_SELECTABLE) {
					menu[selected_item].flags &= ~FLAG_SELECTED;
					selected_item = tmp;
					menu[selected_item].flags |= FLAG_SELECTED;
					break;
				}
			}
		}
	}

	if (key_pressed(ALLEGRO_KEY_ENTER) || key_pressed(ALLEGRO_KEY_SPACE)) {
		if (selected_item != -1 && menu[selected_item].handler != NULL) {
			return menu[selected_item].handler(&menu[selected_item], MSG_INPUT, 0);
		}
	}

	return MENU_CONTINUE;
}

void menu_draw(MENU_ITEM* menu)
{
	draw_box(box);

	for (int i = 0; i < item_count; ++i) {
		if (menu[i].handler) {
			menu[i].handler(&menu[i], MSG_DRAW, 0);
		}
	}	
}

int menu_text_handler(MENU_ITEM* item, int msg, int param) {
	switch (msg) {

	case MSG_INIT:
		item->w = al_get_text_width(menu_font, item->text);
		item->h = al_get_font_line_height(menu_font);
		break;

	case MSG_DRAW:
		al_draw_text(menu_font, al_map_rgb(255, 255, 255), item->x, item->y, ALLEGRO_ALIGN_CENTER, item->text);
		break;
	}

}

int menu_space_handler(MENU_ITEM* item, int msg, int param) {
	switch (msg) {

	case MSG_INIT:
		item->w = 0;
		item->h = item->value;
		break;
	}

	return MENU_CONTINUE;
}

int menu_button_handler(MENU_ITEM* item, int msg, int param){
	switch (msg) {

	case MSG_INIT: 
		item->w = al_get_text_width(menu_font, item->text);
		item->h = al_get_font_line_height(menu_font);
		break;

	case MSG_DRAW:
		if (item->flags & FLAG_SELECTED) {
			al_draw_text(menu_font, al_map_rgb(255, 255, 0), item->x, item->y, ALLEGRO_ALIGN_CENTER, item->text);
		}
		else {
			al_draw_text(menu_font, al_map_rgb(255, 255, 255), item->x, item->y, ALLEGRO_ALIGN_CENTER, item->text);
		}
		break;

	case MSG_INPUT:
		if (item->on_click) {
			item->on_click();
		}
		return MENU_CONTINUE;
	}
	return MENU_CONTINUE;
}
