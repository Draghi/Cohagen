#include <stdio.h>

#include "DragonTest.h"

static void dragonInit();
static void dragonDisplay();

static GLuint vao;
static int numIndicesToDraw;
static Shader *dragonShader;

void dragonTest() {
	dragonInit();

	display.setRenderCallback(dragonDisplay);

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

static void dragonInit() {
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