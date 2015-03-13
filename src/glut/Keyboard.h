#ifndef COH_KEYBOARD_H
#define COH_KEYBOARD_H

#include <stdbool.h>

/**
 * The fake object struct for the keyboard.
 * Do not touch this, unless you're sure of what you're doing.
 */
struct Keyboard_S {

	/**
	 * Initializes the glut callbacks and other variables.
	 */
	void(* init)();

	/**
	 * Updates the keyboard's state.
	 * Should always be called either before or after any keyboard related calls.
	 **/
	void(* update)();

	/**
	 * Returns if the given GLUT key is currently pressed.
	 *
	 * @param key The GLUT key to check ('A' or KEY_F1 eg.)
	 * @return if the given GLUT key is currently pressed.
	 */
	bool(* isKeyDown)(const unsigned char);

	/**
	 * Returns if the given GLUT key was pressed since the last update.
	 *
	 * @param key The GLUT key to check ('A' or KEY_F1 eg.)
	 * @return if the given GLUT key was pressed since the last update.
	 *
	 * @todo Check reliability (might not work with fast key presses)
	 */
	bool(* wasKeyPressed)(const unsigned char);

	/**
	 * Returns if the given GLUT key was pressed since the last update.
	 *
	 * @param key The GLUT key to check ('A' or KEY_F1 eg.)
	 * @return if the given GLUT key was released since the last update.
	 *
	 * @todo Check reliability (might not work with fast key presses)
	 */
	bool(* wasKeyReleased)(const unsigned char);
};


/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct Keyboard_S Keyboard;

/**
 * Expose the Keyboard.c's keyboard object.
 */
extern const Keyboard keyboard;

#endif /* SRC_GLUT_KEYBOARD_H_ */
