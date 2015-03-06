#ifndef GLUT_DISPLAY_H
#define GLUT_DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include"GlutCallbacks.h"

/**
 * The fake object struct for the display.
 * Do not touch this, unless you're sure of what you're doing.
 */
struct Display_s {

	/**
	 * Creates a new window from stored variable.
	 *
	 * @param pargc a pointer to the argc main() variable.
	 * @param argv a pointer to the argv main() variable.
	 * @return Whether or not the window creation was attempted (Can't create twice)
	 */
	bool(* createWindow)(int*, char**);

	/**
	 * Attempts to center the window on the screen.
	 */
	void(* doCenterWindow)();

	/**
	 * Sets the major and minor version of the requested openGL context.
	 * Cannot be changed after window creation.
	 *
	 * @param major The major version number of OpenGL (OpenGL x.0 eg.)
	 * @param minor The minor version number of OpenGL (OpenGL 0.x eg.)
	 */
	void(*setOGLVersion)(uint8_t, uint8_t);

	/**
	 * Sets the display mode of the window (Doublebuffer, multisampling etc.)
	 * Cannot be changed once the window is created.
	 *
	 * @param displayMode The bitmasked glut display mode.
	 */
	void(*setDisplayMode)(uint32_t);

	/**
	 * Changes the position of the window.
	 *
	 * @param x The new x-position of the window.
	 * @param y The new y-position of the window.
	 */
	void(*setWindowPos)(int32_t, int32_t);

	/**
	 * Changes the x-position of the window.
	 *
	 * @param x The new x position of the window.
	 */
	void(*setWindowX)(int32_t);

	/**
	 * Changes the y-position of the window.
	 *
	 * @param y The new y position of the window.
	 */
	void(*setWindowY)(int32_t);

	/**
	 * Changes the size of the window.
	 *
	 * @param width The new width of the window.
	 * @param height The new height of the window.
	 */
	void(*setWindowSize)(int32_t, int32_t);

	/**
	 * Changes the width of window.
	 *
	 * @param width The new width of the window.
	 */
	void(*setWindowWidth)(int32_t);

	/**
	 * Changes the height of the window.
	 *
	 * @param height The new height of the window.
	 */
	void(*setWindowHeight)(int32_t);

	/**
	 * Changes the window's title.
	 *
	 * @param title The new title of the window.
	 */
	void(*setWindowTitle)(char[]);

	/**
	 * Sets the main callback functions (Such as Update or Render)
	 *
	 * @param callbacks The GlutMainCallbacks containing the callbacks.
	 */
	void(*setMainCallbacks)(GlutMainCallbacks*);

	/**
	 * Sets the window callback functions (Such as onResize)
	 *
	 * @param callbacks The GlutWindowCallbacks
	 */
	void(*setWindowCallbacks)(GlutWindowCallbacks*);

	/**
	 * Returns the OpenGL major version (OpenGL x.3 eg.)
	 * @return The OpenGL major version
	 */
	int8_t (*getOGLMajorVer)();

	/**
	 * Returns the OpenGL minor version (OpenGL 3.x eg.)
	 * @return The OpenGL minor version
	 */
	int8_t (*getOGLMinorVer)();

	/**
	 * Returns the window's height
	 * @return The window's height
	 */
	int32_t (*getWindowHeight)();

	/**
	 * Returns the window's width.
	 * @return The window's width
	 */
	int32_t (*getWindowWidth)();

	/**
	 * Returns the window's x-position.
	 * @return The window's x-position
	 */
	int32_t (*getWindowX)();

	/**
	 * Returns the windows y-position.
	 * @return The windows y-position
	 */
	int32_t (*getWindowY)();

	/**
	 * Returns the window's title.
	 * @return The window's title
	 */
	const char* (*getWindowTitle)();

	/**
	 * Returns the screen's width.
	 * @return The screen's width.
	 */
	int32_t(* getScreenWidth)();

	/**
	 * Returns the screen's height.
	 * @return The screen's height.
	 */
	int32_t(* getScreenHeight)();
};

/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct Display_s Display;

/**
 * Expose the Display.c's keyboard object.
 */
extern const Display display;

#endif /* SRC_GLUT_DISPLAY_H_ */
