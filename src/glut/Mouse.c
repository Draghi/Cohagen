/**
 * The Mouse unit monitors the glut mouse events and provides various methods to check the state of the mouse.
 */

#define MAX_MOUSE_BUTTONS 3

#include <stdbool.h>
#include"../graphics.h"
#include "../input.h"

static int32_t xPos;
static int32_t yPos;

static int32_t xDiff;
static int32_t yDiff;

static int32_t cntScrollUp;
static int32_t cntScrollDown;

static bool buttons[MAX_MOUSE_BUTTONS];

/**
 * Glut callback for both the passive and active movements of the mouse.
 *
 * Active meaning a mouse button is down (click and drag eg.)
 * Passive being no button is pressed.
 *
 * Note: Hidden function
 *
 * @param x - the current x position of the mouse.
 * @param y - the current y position of the mouse.
 */
static void onMouseMove(int x, int y) {
	xDiff = x-xPos;
	yDiff = y-yPos;

	xPos = x;
	yPos = y;
}

/**
 * Glut callback for mouse button state changes.
 *
 * Note: Hidden function
 *
 * @param button The glut mouse button.
 * @param state The state the button is now in.
 * @param x The current x position of the mouse.
 * @param y The current y position of the mouse.
 */
static void onMouseButton(int button, int state, int x, int y) {

	if (button<3) {
		if (state==GLUT_DOWN)
			buttons[button] = true;
		else
			buttons[button] = false;
	} else {
		if (state==GLUT_DOWN) {
			if (button==3)
				cntScrollUp++;
			else if (button==4)
				cntScrollDown++;
		}
	}
}

/////////////////////
// Class Functions //
/////////////////////

/**
 * Updates the mouse's state.
 * Should always be called either before or after any mouse related calls.
 **/
static void update() {
	xDiff = 0;
	yDiff = 0;

	cntScrollUp = 0;
	cntScrollDown = 0;
}

/**
 * Initializes the mouse variables and sets the callback functions.
 **/
static void init() {
    for(int i = 0; i<MAX_MOUSE_BUTTONS; i++)
    	buttons[i] = false;

    xPos = 0;
    yPos = 0;

    update();

    glutPassiveMotionFunc(onMouseMove);
    glutMotionFunc(onMouseMove);
    glutMouseFunc(onMouseButton);
}

/////////////
// Getters //
/////////////

/**
 * Checks if the given glut button is pressed or not.
 * @param button The glut button to be checked.
 * @return Whether the given button is pressed or not
 */
static bool isButtonDown(const int32_t button) {
	if ((button<MAX_MOUSE_BUTTONS) && (button>=0))
		return buttons[button];
	else
		return false;
}

/**
 * Returns the number of clicks the mouse wheel has scrolled up.
 * @return The number of clicks the mouse wheel has scrolled up
 **/
static int32_t getScrollUp() {
	return cntScrollUp;
}

/**
 * Returns the number of clicks the mouse wheel has scrolled down.
 * @return The number of clicks the mouse wheel has scrolled down
 **/
static int32_t getScrollDown() {
	return cntScrollDown;
}

/**
 * Returns the number of clicks the mouse wheel has scrolled, positive is up and negative is down.
 * @return The number of clicks the mouse wheel has scrolled, positive is up and negative is down
 **/
static int32_t getScrollCount() {
	return cntScrollUp-cntScrollDown;
}

/**
 * Returns the current x position of the mouse, relative to the window.
 * @return The current x position of the mouse, relative to the window
 **/
static int32_t getXPos() {
	return xPos;
}

/**
 * Returns the current y position of the mouse, relative to the window.
 * @return The current y position of the mouse, relative to the window
 **/
static int32_t getYPos() {
	return yPos;
}

/**
 * Returns the distance the mouse has moved along the x axis since the last update.
 * @return The distance the mouse has moved along the x axis since the last update
 **/
static int32_t getXMove() {
	return xDiff;
}

/**
 * Returns the distance the mouse has moved along the y axis since the last update.
 * @return The distance the mouse has moved along the y axis since the last update.
 */
static int32_t getYMove() {
	return yDiff;
}

///////////////////
// Singleton Def //
///////////////////
/**
 * The instance of the "fake object" singleton for the Mouse
 * Each element corresponds to the strut defined in the header, in order.
 * Do not, I repeat DO NOT mess with this object, unless you are certain about what you're doing.
 */

const Mouse mouse = {init, update, isButtonDown, getScrollUp, getScrollDown, getScrollCount, getXPos, getYPos, getXMove, getYMove};
