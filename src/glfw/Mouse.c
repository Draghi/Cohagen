#include "Mouse.h"

#include "Display.h"
#include <stdlib.h>
#include <stdbool.h>

const int MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1;
const int MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2;
const int MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3;
const int MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4;
const int MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5;
const int MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6;
const int MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7;
const int MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8;

const int MOUSE_BUTTON_LEFT   = GLFW_MOUSE_BUTTON_LEFT;
const int MOUSE_BUTTON_RIGHT  = GLFW_MOUSE_BUTTON_RIGHT;
const int MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;

/**Constructor to build a Mouse**/
static Mouse* new() {
	Mouse* result = malloc(sizeof(Mouse));

	result->oamx = 0;
	result->oamy = 0;
	result->omx = 0;
	result->omy = 0;

	return result;
}

/**Deconstructor to ensure that the parent window is destroyed**/
static void delete(Mouse* mouse) {
	free(mouse);
}

/**Returns the absolute x and y position of the mouse (relative to the monitor) [0] - x, [1] - y**/
static void getAbsolutePosition(Window* window, double* pos) {
	if (manWin.isOpen(window)){
		glfwGetCursorPos(window->window, &pos[0], &pos[1]);
		pos[0] += manWin.getX(window);
		pos[1] += manWin.getY(window);
	}
}

/**Polls mouse related "things"**/
static void update(Window* window) {
	glfwGetCursorPos(window->window, &window->mouse->omx, &window->mouse->omy);
	double pos[2];
	getAbsolutePosition(window, pos);
	window->mouse->oamx = pos[0];
	window->mouse->oamy = pos[1];
}

/* ************* *
 * Polling Funcs *
 * ************* */
/**Returns true if the given mouse button is down**/
static bool isDown(Window* window, int button) {
	if (manWin.isOpen(window))
		return glfwGetMouseButton(window->window, button) == GLFW_PRESS;

	return false;
}

/**Returns true if the given mouse button is up**/
static bool isUp(Window* window, int button) {
	if (manWin.isOpen(window))
		return glfwGetMouseButton(window->window, button) == GLFW_RELEASE;

	return false;
}

/**Returns the x and y position of the mouse (relative to the window) [0] - x, [1] - y**/
static void getPosition(Window* window, double* pos) {
	if (manWin.isOpen(window))
		glfwGetCursorPos(window->window, &pos[0], &pos[1]);
}

/* ************ *
 * Helper Funcs *
 * ************ */
/**Returns the x position of the mouse relative to the window**/
static double getX(Window* window) {
	double pos[2];
	getPosition(window, pos);
	return pos[0];
}

/**Returns the y position of the mouse relative to the window**/
static double getY(Window* window) {
	double pos[2];
	getPosition(window, pos);
	return pos[1];
}


/**Returns the absolute x position of the mouse relative to the monitor**/
static double getAbsoluteX(Window* window) {
	double pos[2];
	getAbsolutePosition(window, pos);
	return pos[0];
}

/**Returns the absolute y position of the mouse relative to the monitor**/
static double getAbsoluteY(Window* window) {
	double pos[2];
	getAbsolutePosition(window, pos);
	return pos[1];
}

/**Returns the x movement of the mouse since the last update, relative to the display.**/
static double getDX(Window* window) {
	return getX(window)-window->mouse->omx;
}

/**Returns the y movement of the mouse since the last update, relative to the display.**/
static double getDY(Window* window) {
	return getY(window)-window->mouse->omy;
}

/**Returns the x movement of the mouse since the last update, relative to the screen.**/
static double getAbsoluteDX(Window* window) {
	return getAbsoluteX(window)-window->mouse->oamx;
}

/**Returns the y movement of the mouse since the last update, relative to the screen.**/
static double getAbsoluteDY(Window* window) {
	return getAbsoluteY(window)-window->mouse->oamy;
}

/**Returns if the mouse/cursor is hovering inside the window.**/
static bool isInWindow(Window* window) {
	double mouse[2];
	getPosition(window, mouse);
	return ((mouse[0]>=0) && (mouse[0]<manWin.getDisplayWidth(window))) && ((mouse[1]>=0) && (mouse[1]<manWin.getDisplayHeight(window)));
}

const MouseManager manMouse = {new,delete,getAbsolutePosition,update,isDown,isUp,getPosition,getX,getY,getAbsoluteX,getAbsoluteY,getDX,getDY,getAbsoluteDX,getAbsoluteDY,isInWindow};
