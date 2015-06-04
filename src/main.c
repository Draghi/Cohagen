#include "lib/ogl.h"
#include "lib/glfw3.h"
#include "game/GameMain.h"

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
    //runGameLoopTest();
    //runGravity();
    runGame();

    glfwTerminate();

	return 0;
}
