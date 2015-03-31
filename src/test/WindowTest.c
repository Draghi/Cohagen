#include "Tests.h"

#include "../lib/ogl.h"
#include "../glut/Display.h"
#include "../util/OGLUtil.h"

#include <GL/glut.h>
#include <stdio.h>

void update(uint32_t delta) {
	char buf[64];
	snprintf(buf, 64, "FPS: %d | TPS: %d | Ticks/Frames: %f", display.getFPS(), display.getTPS(), display.getTPS()/(float)display.getFPS());
	display.setWindowTitle(buf);
}

void render(uint32_t delta) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

static void setupDisplay() {
	display.setWindowSize(800, 600); //800x600 window
	display.doCenterWindow(); //Make sure the window pops up in the center

	display.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE); //Double buffered, RGB + depth buffer, Multisampled window.
	display.setOGLVersion(3, 3); //Set the context to OGL 3.3

	display.setVirtualTPS(60); //Sets the virtual FPS
	display.setVirtualTPS(120); //Sets the virtual TPS

	display.setRenderCallback(render); //Sets the render callback
	display.setUpdateCallback(update); //Sets the update callback

	display.createWindow(); //Create the window
}

static void setupOpenGL(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    manOGLUtil.setBackfaceCulling(GL_CCW);
}


void runWindowTest() {
	setupDisplay();
	setupOpenGL();
	glutMainLoop();
}
