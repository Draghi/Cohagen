
#include "graphics.h"
#include "test/Tests.h"

#include <time.h>

static void setupLibraries(int argc, char **argv) {
	glutInit(&argc, argv);
}

static void setupDisplay(int argc, char **argv) {
	display.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE);
	display.setWindowSize(800, 600); //800x600 window
	display.doCenterWindow(); //Make sure the window pops up in the center
	display.setOGLVersion(3, 0); //Set the context to OGL 3.0 (set to 3.3 when we move to shader only)
	//display.setWindowTitle("Window"); //Unnecessary to be honest

	//Tell glut to create our window
	display.createWindow(&argc, argv);
}

static void setupOpenGL(int argc, char **argv){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glSetBackfaceCulling(GL_CCW);
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));

	setupLibraries(argc, argv);
	setupDisplay(argc, argv);
	setupOpenGL(argc, argv);

	runTeapotCubes();

	return 0;
}
