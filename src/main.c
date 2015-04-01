#include <stdio.h>
#include <time.h>

#include "render/MatrixManager.h"
#include "test/Tests.h"

// DON'T MOVE THE LOCATION OF THIS INCLUDE
#include <GL/freeglut.h>
#include <stdio.h>

static void setupLibraries(int argc, char **argv) {
	manMat.init();
	glutInit(&argc, argv);
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));
	setupLibraries(argc, argv);

	runDragonTest();
	//runWindowTest();

	glutMainLoop();

	return 0;
}
