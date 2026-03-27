#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <allegro5/allegro5.h>
#include <string.h> // memset 사용을 위해 필요

// 키 상태 정의
#define KEY_SEEN     1
#define KEY_DOWN     2


// 키 상태를 저장할 배열 (외부에서 접근 가능하도록 선언)
extern unsigned char key[ALLEGRO_KEY_MAX];

// 함수 선언
void keyboard_init();
void keyboard_update(ALLEGRO_EVENT* event);

bool key_down(int keycode);
bool key_pressed(int keycode);

#endif