/**
 * The Display unit provides various helper functions via a fake object.
 * Most helper function simply assist in setting up and manipulating the glut window.
 * It also provides an intermediate callback that is used to ensure certain calls or provide extra features.
 */

#include "Display.h"

#include <stdbool.h>

#include "lib/ogl.h"
#include "glut/Keyboard.h"
#include "glut/Mouse.h"

#include <GL/freeglut.h>

static void setWindowPos(const int32_t x, const int32_t y);

/** Whether or not the main window has been created or not. **/
static bool isCreated = false;

/** The number corresponding to the OpenGL major version (OpenGL x.3 eg.) **/
static uint8_t glMajor = 3;
/** The number corresponding to the OpenGL minor version (OpenGL 3.x eg.) **/
static uint8_t glMinor = 3;

/** The display mode for the window (Multi-sampling, rgba etc.) **/
static uint32_t dispMode = GLUT_SINGLE | GLUT_RGB;

/** The x position of the window. */
static int32_t winX = 0;
/** The y position of the window. */
static int32_t winY = 0;

/** The width of the window. */
static int32_t winWidth = 800;
/** The height of the window. */
static int32_t winHeight = 600;

/** The title of the window. */
static const char const* winTitle;

/** The callback for the render loop. */
static void(* renderCallback)(uint32_t) = NULL;
/** The callback for the update loop. */
static void(* updateCallback)(uint32_t) = NULL;

/** Used for calculating the FPS */
static int32_t renderFrames=0, renderTime=0, renderTimeBase=0;
/** The last calculated FPS */
static int32_t fps = 0;

/** Used for calculating the TPS */
static int32_t updateFrames=0, updateTime=0, updateTimeBase=0;
/** The last calculated TPS */
static int32_t tps = 0;

/** Settings **/
static float targetFrameTime = 1/60.0f;
static float targetUpdateTime = 1/60.0f;
static float accum = 0;

///////////////////////
// Private Functions //
///////////////////////
/**
 * Causes the window to resize to the display's size variables.
 */
static void resizeWindow() {
	if (isCreated)
		glutReshapeWindow(winWidth, winHeight);
}

/**
 * Causes the window to reposition to the display's position variables.
 */
static void repositionWindow() {
	if (isCreated)
		glutPositionWindow(winX, winY);
}

/**
 * Updates the TPS counter.
 */
static void updateTPS() {
	updateFrames++;
	updateTime=glutGet(GLUT_ELAPSED_TIME);
	if (updateTime-updateTimeBase>=1000) {
		tps = updateFrames*1000.0/(updateTime-updateTimeBase);
		updateTimeBase = updateTime;
		updateFrames = 0;
	}
}

/**
 * Updates the FPS Counter.
 */
static void updateFPS() {
	renderFrames++;
	renderTime=glutGet(GLUT_ELAPSED_TIME);
	if (renderTime-renderTimeBase>=1000) {
		fps = renderFrames*1000.0/(renderTime-renderTimeBase);
		renderTimeBase = renderTime;
		renderFrames = 0;
	}
}

static void onUpdate() {
	accum += targetFrameTime;
	while(accum>=targetUpdateTime) {
		updateTPS();
		if (updateCallback!=NULL)
			updateCallback(targetUpdateTime);

		mouse.update();
		keyboard.update();
		accum -= targetUpdateTime;
	}

	glutPostRedisplay();
}

static void onRender() {
	updateFPS();

	if (renderCallback!=NULL)
		renderCallback(targetFrameTime);

	glFlush();
	glutSwapBuffers();
}

static void onReshape(const int width, const int height) {
	winWidth = width;
	winHeight = height;
}

///////////////////////
// Class Functions //
///////////////////////
static bool createWindow() {
	if (!isCreated) {

		//Setup and create our window
		glutInitContextVersion(glMajor, glMinor);
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

	    renderTimeBase = glutGet(GLUT_ELAPSED_TIME);
	    updateTimeBase=glutGet(GLUT_ELAPSED_TIME);

	    //Make sure we don't try to create another window.
		isCreated = true;

		return true;
	} else {
		return false;
	}
}

static void doCenterWindow() {
	int32_t scrW = glutGet(GLUT_SCREEN_WIDTH);
	int32_t scrH = glutGet(GLUT_SCREEN_HEIGHT);

	setWindowPos((scrW-winWidth)/2, (scrH-winHeight)/2);
}

///////////////////
// Class Setters //
///////////////////
static void setOGLVersion(const uint8_t major, const uint8_t minor) {
	if (!isCreated) {
		glMajor = major;
		glMinor = minor;
	}
}

static void setDisplayMode(const uint32_t displayMode) {
	if (!isCreated)
		dispMode = displayMode;
}

static void setWindowPos(const int32_t x, const int32_t y) {
	winX = x;
	winY = y;

	repositionWindow();
}

static void setWindowX(const int32_t x) {
	winX = x;

	repositionWindow();
}

static void setWindowY(const int32_t y) {
	winY = y;

	repositionWindow();
}

static void setWindowSize(const int32_t width, const int32_t height) {
	winWidth = width;
	winHeight = height;

	resizeWindow();
}

static void setWindowWidth(const int32_t width) {
	winWidth = width;

	resizeWindow();
}

static void setWindowHeight(const int32_t height) {
	winHeight = height;

	resizeWindow();
}

static void setWindowTitle(const char const title[]) {
	winTitle = title;

	if (isCreated)
		glutSetWindowTitle(winTitle);
}

static void setRenderCallback(void(* callback)(uint32_t)) {
	renderCallback = callback;
}

static void setUpdateCallback(void(* callback)(uint32_t)) {
	updateCallback = callback;
}

static void setVirtualTPS(float updateSpeed) {
	targetUpdateTime = 1/updateSpeed;
}

static void setVirtualFPS(float frameSpeed) {
	targetFrameTime = 1/frameSpeed;
}

/////////////
// Getters //
/////////////
static int8_t getOGLMajorVer() {
	return glMajor;
}

static int8_t getOGLMinorVer() {
	return glMinor;
}

static int32_t getWindowHeight() {
	return winHeight;
}

static int32_t getWindowWidth() {
	return winWidth;
}

static int32_t getWindowX() {
	return winX;
}

static int32_t getWindowY() {
	return winY;
}

static const char* getWindowTitle() {
	return winTitle;
}

static int32_t getScreenWidth() {
	return glutGet(GLUT_SCREEN_WIDTH);
}

static int32_t getScreenHeight() {
	return glutGet(GLUT_SCREEN_HEIGHT);
}

static int32_t getFPS() {
	return fps;
}

static int32_t getTPS() {
	return tps;
}

const Display display = {createWindow, doCenterWindow, setOGLVersion, setDisplayMode, setWindowPos, setWindowX, setWindowY, setWindowSize, setWindowWidth, setWindowHeight, setWindowTitle, setUpdateCallback, setRenderCallback, setVirtualTPS, setVirtualFPS, getOGLMajorVer, getOGLMinorVer, getWindowHeight, getWindowWidth, getWindowX, getWindowY, getWindowTitle, getScreenWidth, getScreenHeight, getFPS, getTPS};

