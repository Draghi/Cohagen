#ifndef COH_DISPLAY_H
#define COH_DISPLAY_H

#include <stdbool.h>

typedef struct Mouse_s Mouse;

#include "Mouse.h"

#include "lib/ogl.h"
#include "lib/glfw3.h"

typedef struct Window_s {
	/** The glfw window reference **/
	GLFWwindow* window;

	Mouse* mouse;

	/** The Window's Width. **/
	int width;

	/** The Window's Height. **/
	int height;

	/** The x position of the window **/
	int x;

	/** The y position of the window **/
	int y;

	/** The title of the window **/
	char* title;
} Window;

typedef struct WindowManager_s {
	/**
	 * Checks if the window is still open.
	 *
	 * @return if the window is open, true, or if the window is closed or closing then false.
	 **/
	bool(* isOpen)(Window* window);

	/**
	 * Causes the OpenGL context to be made current on the caller's thread.
	 *
	 * @return If the context was made current.
	 **/
	bool(* makeContextCurrent)(Window* window);

	/** Constructor for the window. Does not create a window. **/
	Window*(* new)();

	void(* delete)(Window* window);

	/* ************* *
	 * Control Funcs *
	 * ************* */

	/**
	 * Creates a new window with an OpenGL 3.2 context.
	 * You cannot create a window twice.
	 *
	 * @return Whether the window was created successfully.
	 **/
	bool(* openWindow)(Window* window);

	/**
	 * Updates the window's state.
	 *
	 * Polls the mouse/keyboard.
	 **/
	void(* update)(Window* window);

	/**
	 * Swaps the window's buffers,
	 **/
	void(* swapBuffers)(Window* window);

	/**
	 * Tells the window to close.
	 **/
	void(* close)(Window* window);

	/* ************* *
	 * Polling Funcs *
	 * ************* */

	/**
	 * Gets the backbuffer's size.
	 * Use for anything related to graphics.
	 *
	 * @return an array with the first element as width and the second as height of the backbuffer.
	 **/
	void(* getDisplaySize)(Window* window, int* size);

	/**
	 * Gets the monitor's size.
	 * Only use in window manipulation.
	 *
	 * @return an array with the first element as width and the second as height of the monitor.
	 **/
	void(* getMonitorSize)(Window* window, int* size);

	/* ************ *
	 * Setter Funcs *
	 * ************ */
	/**
	 * Changes the position of the window.
	 *
	 * @param x The new x position of the window.
	 * @param y The new y position of the window.
	 **/
	void(* setPosition)(Window* window, int x, int y);

	/**
	 * Changes the size of the window.
	 *
	 * @param x The new width of the window.
	 * @param y The new height of the window.
	 **/
	void(* setSize)(Window* window, int width, int height);

	/**
	 * Changes the title of the window.
	 *
	 * @param title The new title for the window.
	 **/
	void(* setTitle)(Window* window, char* title);

	/* ************ *
	 * Helper Funcs *
	 * ************ */
	/**
	 * Gets the width of the display.
	 * Use for anything related to graphics.
	 *
	 * @return the width of the window.
	 **/
	int(* getDisplayWidth)(Window* window);

	/**
	 * Gets the height of the display.
	 * Use for anything related to graphics.
	 *
	 * @return the height of the window.
	 **/
	int(* getDisplayHeight)(Window* window);

	/**
	 * Gets the width of the display.
	 * Only use for window maniuplation.
	 *
	 * @return the width of the window.
	 **/
	int(* getMontiorWidth)(Window* window);

	/**
	 * Gets the height of the display.
	 * Only use for window maniuplation.
	 *
	 * @return the height of the window.
	 **/
	int(* getMonitorHeight)(Window* window);

	/* ************ *
	 * Getter Funcs *
	 * ************ */
	/**
	 * Gets the window's position.
	 *
	 * @return an array with the first element as x and the second as y of the monitor.
	 **/
	void(* getSize)(Window* window, int* size);

	/**
	 * Gets the window's position.
	 *
	 * @return an array with the first element as width and the second as height of the monitor.
	 **/
	void(* getPos)(Window* window, int* pos);

	/**
	 * Gets the window's x position.
	 *
	 * @return the window's x position.
	 **/
	int(* getX)(Window* window);

	/**
	 * Gets the window's y position.
	 *
	 * @return the window's y position.
	 **/
	int(* getY)(Window* window);

	/**
	 * Gets the window's width.
	 *
	 * @return the window's width.
	 **/
	int(* getWidth)(Window* window);

	/**
	 * Gets the window's height.
	 *
	 * @return the window's height.
	 **/
	int(* getHeight)(Window* window);

	/**
	 * Centers the window on the screen.
	 */
	void(* centerWindow)(Window* window);
} WindowManager;

extern const WindowManager manWin;

#endif /* GLFW_DISPLAY_H_ */
