/**
 * The Display unit provides various helper functions via a fake object.
 * Most helper function simply assist in setting up and manipulating the glut window.
 * It also provides an intermediate callback that is used to ensure certain calls or provide extra features.
 */

#include "Display.h"

#include"../lib/ogl.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <stdbool.h>
#include <GL/freeglut.h>

/** Whether or not the main window has been created or not. **/
static bool isCreated = false;

/** The number corresponding to the OpenGL major version (OpenGL x.3 eg.) **/
static uint8_t glMajor = 3;
/** The number corresponding to the OpenGL minor version (OpenGL 3.x eg.) **/
static uint8_t glMinor = 3;

/** The display mode for the window (Multi-sampling, rgba etc.) **/
static uint32_t dispMode = 0;

/** The x position of the window. */
static int32_t winX = 0;
/** The y position of the window. */
static int32_t winY = 0;

/** The width of the window. */
static int32_t winWidth = 0;
/** The height of the window. */
static int32_t winHeight = 0;

/** The title of the window. */
static const char const* winTitle;

/** The callbacks for the update loop and the render loop etc.. */
static void(* renderCallback)() = NULL;
/** The callbacks for the resizing etc. */
static void(* updateCallback)() = NULL;

/** Used for calculating the FPS */
static int32_t frame=0, time=0, timebase=0;

/** The last calculated FPS */
static int32_t fps;

//Just a note on the callback struts, the purpose of them is to allow us to add extra callbacks too them,
//  without having to rewrite function params and everywhere we call said functions.

///////////////////////
// Private Functions //
///////////////////////

/**Internally used function to cause the window to resize to the stored values.*/
static void resizeWindow() {
	if (isCreated)
		glutReshapeWindow(winWidth, winHeight);
}

/**Internally used function to cause the window to resize to the stored values. */
static void repositionWindow() {
	if (isCreated)
		glutPositionWindow(winX, winY);
}

/**
 * Used as the glut idle callback, forwards the call onto the set callback.
 * Used to ensure that certain calls are made.
 *
 * @todo Add TPS/FPS managing capabilities. (120 updates, 60 renders a second eg)
 */
static void onUpdate() {
	if (updateCallback!=NULL)
		updateCallback();

	glutPostRedisplay();

	mouse.update();
	keyboard.update();
}

/**
 * Used as the glut display callback, forwards the call onto the set callback.
 * Used to ensure that certain calls are made.
 */
static void onRender() {
	if (renderCallback!=NULL)
		renderCallback();

	glFlush();
	glutSwapBuffers();

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time-timebase>=1000) {
		fps = frame*1000.0/(time-timebase);
		timebase = time;
		frame = 0;
	}
}

/**
 * Used as the glut reshape callback, forwards the call onto the set callback.
 * Used to ensure that the displays internal width is updated.
 *
 * @param width The new width  of the window.
 * @param height The new height of the window.
 */
static void onReshape(const int width, const int height) {
	winWidth = width;
	winHeight = height;
}

///////////////////////
// Class Functions //
///////////////////////
/**
 * Creates a new window from stored variable.
 *
 * @param pargc a pointer to the argc main() variable.
 * @param argv a pointer to the argv main() variable.
 * @return Whether or not the window creation was attempted (Can't create twice)
 */
static bool createWindow() {
	if (!isCreated) {
		//Setup and create our window
		glutInitContextVersion(glMajor, glMinor);
		glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
		glutInitDisplayMode(dispMode);
		glutInitWindowPosition(winX, winY);
	    glutInitWindowSize(winWidth, winHeight);
	    glutCreateWindow(winTitle);

	    //glewInit();
	    ogl_LoadFunctions();
	    mouse.init();
	    keyboard.init();

	    //Sets all the callback functions to the internal display ones.
	    glutIdleFunc(onUpdate);
	    glutDisplayFunc(onRender);
	    glutReshapeFunc(onReshape);

	    //Make sure we don't try to create another window.
		isCreated = true;

		return true;
	} else {
		return false;
	}
}

/**
 * Attempts to center the window on the screen.
 */
static void doCenterWindow() {
	int32_t scrW = glutGet(GLUT_SCREEN_WIDTH);
	int32_t scrH = glutGet(GLUT_SCREEN_HEIGHT);

	display.setWindowPos((scrW-winWidth)/2, (scrH-winHeight)/2);
}

///////////////////
// Class Setters //
///////////////////
/**
 * Sets the major and minor version of the requested openGL context.
 * Cannot be changed after window creation.
 *
 * @param major The major version number of OpenGL (OpenGL x.0 eg.)
 * @param minor The minor version number of OpenGL (OpenGL 0.x eg.)
 */
static void setOGLVersion(const uint8_t major, const uint8_t minor) {
	if (!isCreated) {
		glMajor = major;
		glMinor = minor;
	}
}

/**
 * Sets the display mode of the window (Doublebuffer, multisampling etc.)
 * Cannot be changed once the window is created.
 *
 * @param displayMode The bitmasked glut display mode.
 */
static void setDisplayMode(const uint32_t displayMode) {
	if (!isCreated)
		dispMode = displayMode;
}

/**
 * Changes the position of the window.
 *
 * @param x The new x-position of the window.
 * @param y The new y-position of the window.
 */
static void setWindowPos(const int32_t x, const int32_t y) {
	winX = x;
	winY = y;

	repositionWindow();
}

/**
 * Changes the x-position of the window.
 *
 * @param x The new x position of the window.
 */
static void setWindowX(const int32_t x) {
	winX = x;

	repositionWindow();
}

/**
 * Changes the y-position of the window.
 *
 * @param y The new y position of the window.
 */
static void setWindowY(const int32_t y) {
	winY = y;

	repositionWindow();
}

/**
 * Changes the size of the window.
 *
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
static void setWindowSize(const int32_t width, const int32_t height) {
	winWidth = width;
	winHeight = height;

	resizeWindow();
}

/**
 * Changes the width of window.
 *
 * @param width The new width of the window.
 */
static void setWindowWidth(const int32_t width) {
	winWidth = width;

	resizeWindow();
}

/**
 * Changes the height of the window.
 *
 * @param height The new height of the window.
 */
static void setWindowHeight(const int32_t height) {
	winHeight = height;

	resizeWindow();
}

/**
 * Changes the window's title.
 *
 * @param title The new title of the window.
 */
static void setWindowTitle(const char const title[]) {
	winTitle = title;

	if (isCreated)
		glutSetWindowTitle(winTitle);
}

/**
 * Sets the render callback. Called when the screen should be updated.
 *
 * @param callbacks The GlutMainCallbacks containing the callbacks.
 */
static void setRenderCallback(void(* callback)()) {
	renderCallback = callback;
}

/**
 * Sets the update callback. Called when the game's state should be updated.
 *
 * @param callbacks The GlutWindowCallbacks
 */
static void setUpdateCallback(void(* callback)()) {
	updateCallback = callback;
}

/////////////
// Getters //
/////////////
/**
 * Returns the OpenGL major version (OpenGL x.3 eg.)
 * @return The OpenGL major version
 */
static int8_t getOGLMajorVer() {
	return glMajor;
}

/**
 * Returns the OpenGL minor version (OpenGL 3.x eg.)
 * @return The OpenGL minor version
 */
static int8_t getOGLMinorVer() {
	return glMinor;
}

/**
 * Returns the window's height
 * @return The window's height
 */
static int32_t getWindowHeight() {
	return winHeight;
}

/**
 * Returns the window's width.
 * @return The window's width
 */
static int32_t getWindowWidth() {
	return winWidth;
}

/**
 * Returns the window's x-position.
 * @return The window's x-position
 */
static int32_t getWindowX() {
	return winX;
}

/**
 * Returns the windows y-position.
 * @return The windows y-position
 */
static int32_t getWindowY() {
	return winY;
}

/**
 * Returns the window's title.
 * @return The window's title
 */
static const char* getWindowTitle() {
	return winTitle;
}

/**
 * Returns the screen's width.
 * @return The screen's width.
 */
static int32_t getScreenWidth() {
	return glutGet(GLUT_SCREEN_WIDTH);
}

/**
 * Returns the screen's height.
 * @return The screen's height.
 */
static int32_t getScreenHeight() {
	return glutGet(GLUT_SCREEN_HEIGHT);
}

/**
 * Returns the FPS.
 * @return The FPS.
 */
static int32_t getFPS() {
	return fps;
}

///////////////////
// Singleton Def //
///////////////////
/**
 * The instance of the "fake object" singleton for the Display
 * Each element corresponds to the strut defined in the header, in order.
 * Do not, I repeat DO NOT mess with this object, unless you are certain about what you're doing.
 */
const Display display = {createWindow, doCenterWindow, setOGLVersion, setDisplayMode, setWindowPos, setWindowX, setWindowY, setWindowSize, setWindowWidth, setWindowHeight, setWindowTitle, setUpdateCallback, setRenderCallback, getOGLMajorVer, getOGLMinorVer, getWindowHeight, getWindowWidth, getWindowX, getWindowY, getWindowTitle, getScreenWidth, getScreenHeight, getFPS};

