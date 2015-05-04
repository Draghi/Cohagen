#ifndef COH_DISPLAY_H
#define COH_DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

/**
 * The fake object struct for the display.
 * Do not touch this, unless you're sure of what you're doing.
 */
struct Display_s {
	/**
	 * Creates a glut window from the settings provided by the display.
	 * There may only be one display at any one given time.
	 *
	 * @return If a new window was created or not.
	 */
	bool(* createWindow)();

	/**
	 * Helper method to center the window in one call.
	 */
	void(* doCenterWindow)();

	/**
	 * Changes the target OpenGL major and minor versions.
	 *
	 * @param major The major version number of OpenGL (OpenGL x.0)
	 * @param minor The minor version number of OpenGL (OpenGL 0.x)
	 */
	void(*setOGLVersion)(const uint8_t major, const uint8_t minor);

	/**
	 * Sets the glut display mode.
	 * Only functions prior to window creation.
	 *
	 * @param dispMode The display mode to set glut to.
	 */
	void(*setDisplayMode)(const uint32_t dispMode);

	/**
	 * Changes the x and y position of the window.
	 *
	 * @param x The new x position of the window.
	 * @param y The new y position of the window.
	 */
	void(*setWindowPos)(const int32_t x, const int32_t y);

	/**
	 * Changes the x position of the window.
	 *
	 * @param x The new x position of the window.
	 */
	void(*setWindowX)(const int32_t x);

	/**
	 * Changes the y position of the window.
	 *
	 * @param y The new y position of the window.
	 */
	void(*setWindowY)(const int32_t);

	/**
	 * Changes the width and height of the window
	 * @param width The new width of the window.
	 * @param height The new height of the window.
	 */
	void(*setWindowSize)(const int32_t width, const int32_t height);

	/**
	 * Changes the width of the window
	 * @param width The new width of the window.
	 */
	void(*setWindowWidth)(const int32_t width);

	/**
	 * Changes the height of the window
	 * @param height The new height of the window.
	 */
	void(*setWindowHeight)(const int32_t height);

	/**
	 * Changes the title of the window.
	 * @param title The new title of the window.
	 */
	void(*setWindowTitle)(const char[]);

	/**
	 * Sets the callback to be used when the window is redrawn.
	 * @param callback The callback to be used when the window is redrawn.
	 */
	void(*setRenderCallback)(void(* callback)(uint32_t));

	/**
	 * Sets the callback to be used when the world should be updated.
	 * @param callback The callback to be used when the world should be updated.
	 */
	void(*setUpdateCallback)(void(* callback)(uint32_t));

	/**
	 * Sets the virtual TPS target.
	 * @param tps The target TPS
	 */
	void(* setVirtualTPS)(float tps);

	/**
	 * Sets the virtual FPS target.
	 * @param fps The target FPS
	 */
	void(* setVirtualFPS)(float fps);

	/**
	 * Returns the major openGL version of the context.
	 * @return The major openGL version of the context.
	 */
	int8_t (*getOGLMajorVer)();

	/**
	 * Returns the minor openGL version of the context.
	 * @return The minor openGL version of the context.
	 */
	int8_t (*getOGLMinorVer)();

	/**
	 * Returns the window height
	 * @return The window height
	 */
	int32_t (*getWindowHeight)();

	/**
	 * Returns the window width
	 * @return The window width
	 */
	int32_t (*getWindowWidth)();

	/**
	 * Returns the window x pos
	 * @return The window x pos
	 */
	int32_t (*getWindowX)();

	/**
	 * Returns the window y pos
	 * @return The window y pos
	 */
	int32_t (*getWindowY)();

	/**
	 * Returns the window title
	 * @return The window title
	 */
	const char* (*getWindowTitle)();

	/**
	 * Returns the screen width
	 * @return The screen width
	 */
	int32_t(* getScreenWidth)();

	/**
	 * Returns the screen height
	 * @return The screen height
	 */
	int32_t(* getScreenHeight)();

	/**
	 * Returns the current fps
	 * @return The current fps
	 */
	int32_t(* getFPS)();

	/**
	 * Returns the current tps
	 * @return The current tps
	 */
	int32_t(* getTPS)();
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
