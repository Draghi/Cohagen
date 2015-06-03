#ifndef COH_KEYBOARD_H
#define COH_KEYBOARD_H

#include "Display.h"
#include <stdbool.h>

/**
 *	Keyboard input wrapper.
 */
typedef struct Keyboard_s {
	void* emptyStruct;
} Keyboard;

/**
 *	Class used to manage keyboard input.
 */
typedef struct KeyboardManager_s {
	/**Constructor to build a keyboard**/
	Keyboard*(* new)();

	/** Destructor to ensure the parent window is destroyed. **/
	void(* delete)(Keyboard* keyboard);

	/**Polls the keyboard**/
	void(* update)(Window* window);

	/** Returns true if the given key is down. **/
	bool(* isDown)(Window* window, int key);

	/** Returns true if the given key is up **/
	bool(* isUp)(Window* window, int key);
} KeyboardManager;

extern const KeyboardManager manKeyboard;

extern const int KEY_0;
extern const int KEY_1;
extern const int KEY_2;
extern const int KEY_3;
extern const int KEY_4;
extern const int KEY_5;
extern const int KEY_6;
extern const int KEY_7;
extern const int KEY_8;
extern const int KEY_9;


extern const int KEY_A;
extern const int KEY_B;
extern const int KEY_C;
extern const int KEY_D;
extern const int KEY_E;
extern const int KEY_F;
extern const int KEY_G;
extern const int KEY_H;
extern const int KEY_I;
extern const int KEY_J;
extern const int KEY_K;
extern const int KEY_L;
extern const int KEY_M;
extern const int KEY_N;
extern const int KEY_O;
extern const int KEY_P;
extern const int KEY_Q;
extern const int KEY_R;
extern const int KEY_S;
extern const int KEY_T;
extern const int KEY_U;
extern const int KEY_V;
extern const int KEY_W;
extern const int KEY_X;
extern const int KEY_Y;
extern const int KEY_Z;

extern const int KEY_SPACE;
extern const int KEY_APOSTROPHE; /* ' */
extern const int KEY_COMMA;  /* , */
extern const int KEY_MINUS;  /* - */
extern const int KEY_PERIOD;  /* . */
extern const int KEY_SLASH;  /* / */
extern const int KEY_SEMICOLON;  /* ; */
extern const int KEY_EQUAL;  /* = */
extern const int KEY_LBRACKET;  /* [ */
extern const int KEY_BACKSLASH;  /* \ */
extern const int KEY_RBRACKET;  /* ] */
extern const int KEY_GRAVE;  /* ` */

extern const int KEY_ESC;
extern const int KEY_ENTER;
extern const int KEY_TAB;
extern const int KEY_BACKSPACE;
extern const int KEY_INSERT;
extern const int KEY_DEL;

extern const int KEY_RIGHT;
extern const int KEY_LEFT;
extern const int KEY_DOWN;
extern const int KEY_UP;

extern const int KEY_PAGE_UP;
extern const int KEY_PAGE_DOWN;
extern const int KEY_HOME;
extern const int KEY_END;
extern const int KEY_PRINT_SCREEN;
extern const int KEY_PAUSE;

extern const int KEY_CAPS_LOCK;
extern const int KEY_SCROLL_LOCK;
extern const int KEY_NUM_LOCK;

extern const int KEY_F1;
extern const int KEY_F2;
extern const int KEY_F3;
extern const int KEY_F4;
extern const int KEY_F5;
extern const int KEY_F6;
extern const int KEY_F7;
extern const int KEY_F8;
extern const int KEY_F9;
extern const int KEY_F10;
extern const int KEY_F11;
extern const int KEY_F12;

extern const int KEY_NUM_0;
extern const int KEY_NUM_1;
extern const int KEY_NUM_2;
extern const int KEY_NUM_3;
extern const int KEY_NUM_4;
extern const int KEY_NUM_5;
extern const int KEY_NUM_6;
extern const int KEY_NUM_7;
extern const int KEY_NUM_8;
extern const int KEY_NUM_9;
extern const int KEY_NUM_DECIMAL;
extern const int KEY_NUM_DIV;
extern const int KEY_NUM_MULT;
extern const int KEY_NUM_SUB;
extern const int KEY_NUM_ADD;
extern const int KEY_NUM_ENTER;
extern const int KEY_NUM_EQUAL;
extern const int KEY_ESCAPE;


extern const int KEY_LSHIFT;
extern const int KEY_LCONTROL;
extern const int KEY_LALT;
extern const int KEY_LSUPER;
extern const int KEY_RSHIFT;
extern const int KEY_RCONTROL;
extern const int KEY_RALT;
extern const int KEY_RSUPER;
extern const int KEY_MENU;

#endif
