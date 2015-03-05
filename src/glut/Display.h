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
	bool(* createWindow)(int*, char**);
	void(* doCenterWindow)();

	void(*setOGLVersion)(uint8_t, uint8_t);
	void(*setDisplayMode)(uint32_t);
	void(*setWindowPos)(int32_t, int32_t);
	void(*setWindowX)(int32_t);
	void(*setWindowY)(int32_t);
	void(*setWindowSize)(int32_t, int32_t);
	void(*setWindowWidth)(int32_t);
	void(*setWindowHeight)(int32_t);
	void(*setWindowTitle)(char[]);
	void(*setMainCallbacks)(GlutMainCallbacks*);
	void(*setWindowCallbacks)(GlutWindowCallbacks*);

	int8_t (*getOGLMajorVer)();
	int8_t (*getOGLMinorVer)();
	int32_t (*getWindowHeight)();
	int32_t (*getWindowWidth)();
	int32_t (*getWindowX)();
	int32_t (*getWindowY)();
	const char* (*getWindowTitle)();

	int32_t(* getScreenWidth)();
	int32_t(* getScreenHeight)();
};

/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct Display_s Display;

/**
 * Expose the "singleton" instance.
 */
extern const Display display;

#endif /* SRC_GLUT_DISPLAY_H_ */
