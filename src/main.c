#include "render/MatrixManager.h"
#include "lib/ogl.h"
#include "lib/glfw3.h"
#include "render/MatrixManager.h"
#include "test/Tests.h"
#include "test/HouseTest.h"

#include <stdlib.h>
#include <time.h>

static void setupLibraries(int argc, char **argv) {
	manMat.init();
	glfwInit();
	ogl_LoadFunctions();
	manMat.init();
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));
	setupLibraries(argc, argv);

	// runTexTest();
	runHouseTest();
	// runWindowTest();

	//glutMainLoop();

	glfwTerminate();

	return 0;
}
