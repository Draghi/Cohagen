#include "Tests.h"

#include <stdio.h>

#include "lib/ogl.h"
#include "glfw/Display.h"
#include "util/OGLUtil.h"

#include <GL/glut.h>

Window* window;

void update(uint32_t delta) {
	//char buf[64];
	//snprintf(buf, 64, "FPS: %d | TPS: %d | Ticks/Frames: %f", display.getFPS(), display.getTPS(), display.getTPS()/(float)display.getFPS());
	//manWin.setTitle(buf);
}

void render(uint32_t delta) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

static void setupDisplay() {
	manWin.setSize(window, 800, 600); //800x600 window
	//manWin.doCenterWindow(); //Make sure the window pops up in the center

	//manWin.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE); //Double buffered, RGB + depth buffer, Multisampled window.
	//manWin.setOGLVersion(3, 3); //Set the context to OGL 3.3

	//manWin.setVirtualFPS(60); //Sets the virtual FPS
	//manWin.setVirtualTPS(120); //Sets the virtual TPS

	//manWin.setRenderCallback(render); //Sets the render callback
	//manWin.setUpdateCallback(update); //Sets the update callback

	manWin.openWindow(window); //Create the window
}

static void setupOpenGL(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    manOGLUtil.setBackfaceCulling(GL_CCW);
}


void runWindowTest() {
	window = manWin.new();
	setupDisplay();
	setupOpenGL();
}
