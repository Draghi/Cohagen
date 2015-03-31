
#include "render/MatrixManager.h"
#include "lib/ogl.h"
#include "glut/Display.h"
#include "test/Tests.h"

#include <time.h>
#include <GL/freeglut.h>
#include <stdio.h>

static void setupLibraries(int argc, char **argv) {
	manMat.init();
	glutInit(&argc, argv);
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));
	setupLibraries(argc, argv);

	runWindowTest();

	return 0;
}
