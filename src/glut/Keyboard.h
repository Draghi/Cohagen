/*
 * Keyboard.h
 *
 *  Created on: 6 Mar 2015
 *      Author: draghi
 */

#ifndef SRC_GLUT_KEYBOARD_H_
#define SRC_GLUT_KEYBOARD_H_

#include <stdbool.h>

struct Keyboard_S {
	void(* init)();
	void(* update)();

	bool(* isKeyDown)(unsigned char);
	bool(* wasKeyPressed)(unsigned char);
	bool(* wasKeyReleased)(unsigned char);
};

typedef struct Keyboard_S Keyboard;

extern const Keyboard keyboard;

#endif /* SRC_GLUT_KEYBOARD_H_ */
