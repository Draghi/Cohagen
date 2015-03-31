#include <stdio.h>
#include <time.h>

#include "render/MatrixManager.h"
#include "lib/ogl.h"
#include "glut/Display.h"
#include "util/OGLUtil.h"
#include "test/Tests.h"

// DON'T MOVE THE LOCATION OF THIS INCLUDE
#include <GL/freeglut.h>

static void setupLibraries(int argc, char **argv) {
	manMat.init();
	glutInit(&argc, argv);
}

static void setupDisplay() {
	display.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE);
	display.setWindowSize(800, 600); //800x600 window
	display.doCenterWindow(); //Make sure the window pops up in the center
	display.setOGLVersion(3, 3); //Set the context to OGL 3.3

	//Tell glut to create our window
	display.createWindow();
}

static void setupOpenGL(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    manOGLUtil.setBackfaceCulling(GL_CCW);
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));

	setupLibraries(argc, argv);
	setupDisplay();
	setupOpenGL();

	dragonTest();

	glutMainLoop();

	return 0;
}
