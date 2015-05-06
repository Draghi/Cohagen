#include "engine/MatrixManager.h"
#include "lib/ogl.h"
#include "lib/glfw3.h"
#include "test/Tests.h"

#include <stdlib.h>
#include <time.h>

static void setupLibraries(int argc, char **argv) {
	ogl_LoadFunctions();
	glfwInit();
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));
	setupLibraries(argc, argv);

    runGameLoopTest();

	glfwTerminate();

	return 0;
}
