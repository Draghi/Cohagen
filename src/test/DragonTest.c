#include <stdio.h>

#include "DragonTest.h"

#include <GL/freeglut.h>

static void dragonInit();
static void dragonDisplay();

static GLuint vao;
static int numIndicesToDraw;
static Shader *dragonShader;

void runDragonTest() {
	dragonInit();

	display.setRenderCallback(dragonDisplay);

	printf("My Dragon test\n");
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

static void dragonDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderManager.bind(dragonShader);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shaderManager.unbind();
}

static void dragonInit() {
	setupDisplay();
	setupOpenGL();

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);

	manOGLUtil.setBackfaceCulling(GL_CCW);

	// Load .obj file
	vao = objLoader.genVAOFromFile("./data/models/dragon_smooth.obj", &numIndicesToDraw);

	printf("VAO:%d numIndices:%d", vao, numIndicesToDraw);

	dragonShader = shaderManager.newFromGroup("./data/shaders/", "passThru");
}

// static Mat4 createProjectionMatrix(float verticalFovRad, float nearZClip, float farZClip, float aspectRatio) 
// {
// 	float range = tan(verticalFovRad * 0.5f) * nearZClip;
	
// 	float Sx = (2.0f * nearZClip) / (range * aspectRatio + range * aspectRatio);
// 	float Sy = nearZClip / range;
// 	float Sz = -(farZClip + nearZClip) / (farZClip - nearZClip);
// 	float Pz = -(2.0f * farZClip * nearZClip) / (farZClip - nearZClip);

// 	return (manMat4.create(
// 		NULL,
// 		Sx, 0.0f, 0.0f, 0.0f,
// 		0.0f, Sy, 0.0f, 0.0f,
// 		0.0f, 0.0f, Sz, Pz,
// 		0.0f, 0.0f, -1.0f, 0.0f));
// }