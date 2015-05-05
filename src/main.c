#include "render/MatrixManager.h"
#include "lib/ogl.h"
#include "test/Tests.h"
#include "test/TexTest.h"
#include "test/HouseTest.h"
#include "test/DragonTest.h"

#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

static void setupLibraries(int argc, char **argv) {
	manMat.init();
	glutInit(&argc, argv);
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));
	setupLibraries(argc, argv);

	runDragonTest();
	// runTexTest();
	// runHouseTest();
	// runWindowTest();

	glutMainLoop();

	return 0;
}
