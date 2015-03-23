
#include "lib/ogl.h"
#include "glut/Display.h"
#include "util/OGLUtil.h"
#include "test/Tests.h"

#include <time.h>
#include <GL/freeglut.h>

static void setupLibraries(int argc, char **argv) {
	glutInit(&argc, argv);
}

static void setupDisplay() {
	display.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE);
	display.setWindowSize(800, 600); //800x600 window
	display.doCenterWindow(); //Make sure the window pops up in the center
	display.setOGLVersion(3, 0); //Set the context to OGL 3.0 (set to 3.3 when we move to shader only)

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



	return 0;
}
