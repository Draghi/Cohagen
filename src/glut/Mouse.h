#ifndef COH_MOUSE_H
#define COH_MOUSE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * The fake object struct for the mouse.
 * Do not touch this, unless you're sure of what you're doing.
 */
struct Mouse_s {

	/**
	 * Updates the mouse's state.
	 * Should always be called either before or after any mouse related calls.
	 **/
	void(*init)();

	/**
	 * Initializes the mouse variables and sets the callback functions.
	 **/
	void(*update)();

	/**
	 * Checks if the given glut button is pressed or not.
	 * @param button The glut button to be checked.
	 * @return Whether the given button is pressed or not
	 */
	bool(*isButtonDown)(const int);

	/**
	 * Returns the number of clicks the mouse wheel has scrolled up.
	 * @return The number of clicks the mouse wheel has scrolled up
	 **/
	int32_t(*getScrollUp)();

	/**
	 * Returns the number of clicks the mouse wheel has scrolled down.
	 * @return The number of clicks the mouse wheel has scrolled down
	 **/
	int32_t(*getScrollDown)();

	/**
	 * Returns the number of clicks the mouse wheel has scrolled, positive is up and negative is down.
	 * @return The number of clicks the mouse wheel has scrolled, positive is up and negative is down
	 **/
	int32_t(*getScrollCount)();

	/**
	 * Returns the current x position of the mouse, relative to the window.
	 * @return The current x position of the mouse, relative to the window
	 **/
	int32_t(*getXPos)();

	/**
	 * Returns the current y position of the mouse, relative to the window.
	 * @return The current y position of the mouse, relative to the window
	 **/
	int32_t(*getYPos)();

	/**
	 * Returns the distance the mouse has moved along the x axis since the last update.
	 * @return The distance the mouse has moved along the x axis since the last update
	 **/
	int32_t(*getXMove)();

	/**
	 * Returns the distance the mouse has moved along the y axis since the last update.
	 * @return The distance the mouse has moved along the y axis since the last update.
	 */
	int32_t(*getYMove)();
};

/**
 * Make it easier to define an instance of the Mouse object
 */
typedef struct Mouse_s Mouse;


/**
 * Expose the Mouse.c's mouse object.
 */
extern const Mouse mouse;

#endif /* SRC_GLUT_MOUSE_H_ */
