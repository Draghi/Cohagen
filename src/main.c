#include "lib/ogl.h"
#include "lib/glfw3.h"
#include "test/Tests.h"

#include <stdlib.h>
#include <time.h>

static void setupLibraries(int argc, char **argv) {
	glfwInit();
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));
    setupLibraries(argc, argv);

    //runNewtonsCradle();
    //runPhysicsGLFWTest();
    // runGravity();
    runQuitScreen();

    glfwTerminate();

	return 0;
}
