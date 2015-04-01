#include <stdio.h>

#include "DragonTest.h"

#include "lib/ogl.h"
#include "glut/Display.h"
#include "math/Mat4.h"
#include "util/ObjLoader.h"
#include "util/OGLUtil.h"
#include "gl/Shader.h"

#include <GL/glut.h>
#include <math.h>

static void setupDragonDisplay();
static void setupDragonOpenGL();
static void loadDragonResources();
static void dragonDisplay();
static void dragonUpdate();

static GLuint vao;
static int numIndicesToDraw;
static Shader *dragonShader;

void dragonTest() {
	setupDragonDisplay();
	setupDragonOpenGL();
	loadDragonResources();

	printf("My Dragon test\n");
}

static void dragonDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderManager.bind(dragonShader);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shaderManager.unbind();
}

static void dragonUpdate(uint32_t delta) {
	char buf[64];
	snprintf(buf, 64, "FPS: %d | TPS: %d | Ticks/Frames: %f", display.getFPS(), display.getTPS(), display.getTPS()/(float)display.getFPS());
	display.setWindowTitle(buf);
}

static void setupDragonDisplay() {
	display.setWindowSize(800, 600); //800x600 window
	display.doCenterWindow(); //Make sure the window pops up in the center

	display.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE); //Double buffered, RGB + depth buffer, Multisampled window.
	display.setOGLVersion(3, 3); //Set the context to OGL 3.3

	display.setVirtualTPS(60); //Sets the virtual FPS
	display.setVirtualTPS(120); //Sets the virtual TPS

	display.setUpdateCallback(dragonUpdate);
	display.setRenderCallback(dragonDisplay);

	display.createWindow(); //Create the window
}

static void setupDragonOpenGL() {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	manOGLUtil.setBackfaceCulling(GL_CCW);
}

static void loadDragonResources() {
	vao = objLoader.genVAOFromFile("./data/models/dragon_smooth.obj", &numIndicesToDraw);
	printf("VAO ID: %d Vertex Count: %d\n", vao, numIndicesToDraw);


	dragonShader = shaderManager.newFromGroup("./data/shaders/", "dragon");
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
