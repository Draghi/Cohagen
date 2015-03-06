/*
 * Keyboard.c
 *
 *  Created on: 6 Mar 2015
 *      Author: draghi
 */

#define MAX_KEYBOARD_KEYS 256

#include "Keyboard.h"
#include <stdbool.h>
#include <GL/freeglut.h>


bool keyStates[MAX_KEYBOARD_KEYS];
bool oldKeyStates[MAX_KEYBOARD_KEYS];

/**
 * Glut callback for processing keyboard down events
 *
 * Note - Hidden
 */
static void onKeyDown(unsigned char key, int x, int y) {
	keyStates[key] = true;
}

/**
 * Glut callback for processing keyboard up events
 *
 * Note - Hidden
 */
static void onKeyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}


/**
 * Glut callback for processing 'special' keyboard down events
 *
 * Note - Hidden
 */
static void onSpecKeyDown(int key, int x, int y) {
	keyStates[key] = true;
}

/**
 * Glut callback for processing 'special' keyboard up events
 *
 * Note - Hidden
 */
static void onSpecKeyUp(int key, int x, int y) {
	keyStates[key] = false;
}

/**
 * Initializes the glut callbacks and other variables.
 */
static void init() {
    for(int i = 0; i<MAX_KEYBOARD_KEYS; i++) {
    	keyStates[i] = false;
    	oldKeyStates[i] = false;
    }

    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);

    glutSpecialFunc(onSpecKeyDown);
    glutSpecialFunc(onSpecKeyUp);
}

static void update() {
    for(int i = 0; i<MAX_KEYBOARD_KEYS; i++) {
    	oldKeyStates[i] = keyStates[i];
    }
}

/**
 * Returns if the given GLUT key was pressed.
 * @param key The GLUT key to check ('A' or KEY_F1 eg.)
 * @return if the given GLUT key was pressed.
 */
static bool isKeyDown(unsigned char key) {
	return keyStates[key];
}

static bool wasKeyPressed(unsigned char key) {
	if ((oldKeyStates[key]==false) && (keyStates[key]==true))
		return true;
	else
		return false;
}

static bool wasKeyReleased(unsigned char key) {
	if ((oldKeyStates[key]==true) && (keyStates[key]==false))
		return true;
	else
		return false;
}

///////////////////
// Singleton Def //
///////////////////
/**
 * The instance of the "fake object" singleton for the Keyboard
 * Each element corresponds to the strut defined in the header, in order.
 * Do not, I repeat DO NOT mess with this object, unless you are certain about what you're doing.
 */

const Keyboard keyboard = {init, update, isKeyDown, wasKeyPressed, wasKeyReleased};
