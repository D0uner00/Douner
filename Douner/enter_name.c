#include "enter_name.h"
#include "ui.h"
#include "keyboard.h"
#include <string.h>

//extern ALLEGRO_FONT* input_font;

extern  ALLEGRO_FONT* menu_font;
// 초기화
void name_input_init(NameInput* name)
{
    name->length = 0;
    name->buffer[0] = '\0';
    name->cursor_visible = 1;
    name->done = 0;

    // 중앙 박스 생성
    name->box = make_center_rect(400, 200);
}

void name_input_update(NameInput* name, ALLEGRO_EVENT* event)
{
    // 문자 입력 처리
    if (event->type == ALLEGRO_EVENT_KEY_CHAR) {
        int c = event->keyboard.unichar;

        // 일반 문자 입력
        if (c >= 32 && c <= 126) {
            if (name->length < NAME_MAX - 1) {
                name->buffer[name->length++] = (char)c;
                name->buffer[name->length] = '\0';
            }
        }

        // 백스페이스
        else if (event->keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
            if (name->length > 0) {
                name->length--;
                name->buffer[name->length] = '\0';
            }
        }

        // 엔터 → 입력 완료
        else if (event->keyboard.keycode == ALLEGRO_KEY_ENTER) {
            if (name->length > 0) {
                name->done = 1;
            }
        }
    }
}

void name_input_draw(NameInput* name)
{
    // 박스
    draw_box(name->box);

    // 제목
    draw_center_text("Enter Name", name->box.y + 30, al_map_rgb(255, 255, 255));

    // 입력된 텍스트
    draw_center_text(name->buffer, name->box.y + 100, al_map_rgb(255, 255, 0));

    // 커서 (깜빡임 효과 간단 버전)
    if (name->cursor_visible) {
        char temp[NAME_MAX + 2];
        snprintf(temp, sizeof(temp), "%s_", name->buffer);
        draw_center_text(temp, name->box.y + 100, al_map_rgb(255, 255, 0));
    }
}