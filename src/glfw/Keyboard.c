#include "Keyboard.h"

#include "lib/glfw3.h"

#include <stdlib.h>

/**Constructor to build a keyboard**/
static Keyboard* new() {
	return malloc(sizeof(Keyboard));
}

static void delete(Keyboard* keyboard) {
	free(keyboard);
}

static void update(Window* window) {
	//Do nothing.
}

static bool isDown(Window* window, int key) {
	if (manWin.isOpen(window))
		return glfwGetKey(window->window, key) == GLFW_PRESS;

	return false;
}

static bool isUp(Window* window, int key) {
	if (manWin.isOpen(window))
		return glfwGetKey(window->window, key) == GLFW_RELEASE;

	return false;
}

const KeyboardManager manKeyboard = {new, delete, update, isDown, isUp};

const int KEY_0 = GLFW_KEY_0;
const int KEY_1 = GLFW_KEY_1;
const int KEY_2 = GLFW_KEY_2;
const int KEY_3 = GLFW_KEY_3;
const int KEY_4 = GLFW_KEY_4;
const int KEY_5 = GLFW_KEY_5;
const int KEY_6 = GLFW_KEY_6;
const int KEY_7 = GLFW_KEY_7;
const int KEY_8 = GLFW_KEY_8;
const int KEY_9 = GLFW_KEY_9;


const int KEY_A = GLFW_KEY_A;
const int KEY_B = GLFW_KEY_B;
const int KEY_C = GLFW_KEY_C;
const int KEY_D = GLFW_KEY_D;
const int KEY_E = GLFW_KEY_E;
const int KEY_F = GLFW_KEY_F;
const int KEY_G = GLFW_KEY_G;
const int KEY_H = GLFW_KEY_H;
const int KEY_I = GLFW_KEY_I;
const int KEY_J = GLFW_KEY_J;
const int KEY_K = GLFW_KEY_K;
const int KEY_L = GLFW_KEY_L;
const int KEY_M = GLFW_KEY_M;
const int KEY_N = GLFW_KEY_N;
const int KEY_O = GLFW_KEY_O;
const int KEY_P = GLFW_KEY_P;
const int KEY_Q = GLFW_KEY_Q;
const int KEY_R = GLFW_KEY_R;
const int KEY_S = GLFW_KEY_S;
const int KEY_T = GLFW_KEY_T;
const int KEY_U = GLFW_KEY_U;
const int KEY_V = GLFW_KEY_V;
const int KEY_W = GLFW_KEY_W;
const int KEY_X = GLFW_KEY_X;
const int KEY_Y = GLFW_KEY_Y;
const int KEY_Z = GLFW_KEY_Z;

const int KEY_SPACE      = GLFW_KEY_SPACE;
const int KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE; /* ' */
const int KEY_COMMA      = GLFW_KEY_COMMA;  /* , */
const int KEY_MINUS      = GLFW_KEY_MINUS;  /* - */
const int KEY_PERIOD     = GLFW_KEY_PERIOD;  /* . */
const int KEY_SLASH      = GLFW_KEY_SLASH;  /* / */
const int KEY_SEMICOLON  = GLFW_KEY_SEMICOLON;  /* ; */
const int KEY_EQUAL      = GLFW_KEY_EQUAL;  /* = */
const int KEY_LBRACKET   = GLFW_KEY_LEFT_BRACKET;  /* [ */
const int KEY_BACKSLASH  = GLFW_KEY_BACKSLASH;  /* \ */
const int KEY_RBRACKET   = GLFW_KEY_RIGHT_BRACKET;  /* ] */
const int KEY_GRAVE      = GLFW_KEY_GRAVE_ACCENT;  /* ` */

const int KEY_ESC          = GLFW_KEY_ESCAPE;
const int KEY_ENTER        = GLFW_KEY_ENTER;
const int KEY_TAB          = GLFW_KEY_TAB;
const int KEY_BACKSPACE    = GLFW_KEY_BACKSPACE;
const int KEY_INSERT       = GLFW_KEY_INSERT;
const int KEY_DEL          = GLFW_KEY_DELETE;

const int KEY_RIGHT        = GLFW_KEY_RIGHT;
const int KEY_LEFT         = GLFW_KEY_LEFT;
const int KEY_DOWN         = GLFW_KEY_DOWN;
const int KEY_UP           = GLFW_KEY_UP;

const int KEY_PAGE_UP      = GLFW_KEY_PAGE_UP;
const int KEY_PAGE_DOWN    = GLFW_KEY_PAGE_DOWN;
const int KEY_HOME         = GLFW_KEY_HOME;
const int KEY_END          = GLFW_KEY_END;
const int KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
const int KEY_PAUSE        = GLFW_KEY_PAUSE;

const int KEY_CAPS_LOCK    = GLFW_KEY_CAPS_LOCK;
const int KEY_SCROLL_LOCK  = GLFW_KEY_SCROLL_LOCK;
const int KEY_NUM_LOCK     = GLFW_KEY_NUM_LOCK;

const int KEY_F1  = GLFW_KEY_F1;
const int KEY_F2  = GLFW_KEY_F2;
const int KEY_F3  = GLFW_KEY_F3;
const int KEY_F4  = GLFW_KEY_F4;
const int KEY_F5  = GLFW_KEY_F5;
const int KEY_F6  = GLFW_KEY_F6;
const int KEY_F7  = GLFW_KEY_F7;
const int KEY_F8  = GLFW_KEY_F8;
const int KEY_F9  = GLFW_KEY_F9;
const int KEY_F10 = GLFW_KEY_F10;
const int KEY_F11 = GLFW_KEY_F11;
const int KEY_F12 = GLFW_KEY_F12;

const int KEY_NUM_0       = GLFW_KEY_KP_0;
const int KEY_NUM_1       = GLFW_KEY_KP_1;
const int KEY_NUM_2       = GLFW_KEY_KP_2;
const int KEY_NUM_3       = GLFW_KEY_KP_3;
const int KEY_NUM_4       = GLFW_KEY_KP_4;
const int KEY_NUM_5       = GLFW_KEY_KP_5;
const int KEY_NUM_6       = GLFW_KEY_KP_6;
const int KEY_NUM_7       = GLFW_KEY_KP_7;
const int KEY_NUM_8       = GLFW_KEY_KP_8;
const int KEY_NUM_9       = GLFW_KEY_KP_9;
const int KEY_NUM_DECIMAL = GLFW_KEY_KP_DECIMAL;
const int KEY_NUM_DIV     = GLFW_KEY_KP_DIVIDE;
const int KEY_NUM_MULT    = GLFW_KEY_KP_MULTIPLY;
const int KEY_NUM_SUB     = GLFW_KEY_KP_SUBTRACT;
const int KEY_NUM_ADD     = GLFW_KEY_KP_ADD;
const int KEY_NUM_ENTER   = GLFW_KEY_KP_ENTER;
const int KEY_NUM_EQUAL   = GLFW_KEY_KP_EQUAL;
const int KEY_ESCAPE = GLFW_KEY_ESCAPE;

const int KEY_LSHIFT   = GLFW_KEY_LEFT_SHIFT;
const int KEY_LCONTROL = GLFW_KEY_LEFT_CONTROL;
const int KEY_LALT     = GLFW_KEY_LEFT_ALT;
const int KEY_LSUPER   = GLFW_KEY_LEFT_SUPER;
const int KEY_RSHIFT   = GLFW_KEY_RIGHT_SHIFT;
const int KEY_RCONTROL = GLFW_KEY_RIGHT_CONTROL;
const int KEY_RALT     = GLFW_KEY_RIGHT_ALT;
const int KEY_RSUPER   = GLFW_KEY_RIGHT_SUPER;
const int KEY_MENU     = GLFW_KEY_MENU;
