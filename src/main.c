
#include "glut/Display.h"
#include "graphics.h"
#include "test/Tests.h"

#include <time.h>

static void setupLibraries(int argc, char **argv) {
	glutInit(&argc, argv);
}

static void setupDisplay(int argc, char **argv) {
	display.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE);
	display.setWindowSize(800, 600);
	display.doCenterWindow();
	display.setOGLVersion(3, 0);
	display.setWindowTitle("Window");

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
