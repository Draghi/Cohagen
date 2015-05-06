#include "HouseTest.h"

#include <stdio.h>
#include <math.h>

#include "lib/ogl.h"
#include "glut/Display.h"
#include "util/ObjLoader.h"
#include "util/OGLUtil.h"
#include "util/TextureUtil.h"
#include "gl/Shader.h"
#include "gl/Textures.h"
#include "math/Mat4.h"

#include <GL/glut.h>

static void setupCubeDisplay();
static void setupCubeOpenGL();
static void loadCubeResources();
static void cubeDisplay();
static void cubeUpdate();
static Mat4 createProjectionMatrix(float verticalFovRad, float nearZClip, float farZClip, float aspectRatio);

static GLuint vao;
static int numIndicesToDraw;
static Shader *cubeShader;
static Texture *tex;
static Mat4 projMatrix;
static Mat4 modelMatrix;

void runHouseTest() {
	setupCubeDisplay();
	setupCubeOpenGL();
	loadCubeResources();
}

static void cubeDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE);

	manShader.bind(cubeShader);
		manTex.bind(tex, 0);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		manTex.unbind(tex);
	manShader.unbind();

	glDisable(GL_TEXTURE);
}

static void cubeUpdate(uint32_t delta) {
	glViewport(0, 0, display.getWindowWidth(), display.getWindowHeight());
}

static void setupCubeDisplay() {
	display.setWindowSize(800, 600); //800x600 window
	display.doCenterWindow(); //Make sure the window pops up in the center

	display.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE); //Double buffered, RGB + depth buffer, Multisampled window.
	display.setOGLVersion(3, 3); //Set the context to OGL 3.3

	display.setVirtualFPS(60); //Sets the virtual FPS
	display.setVirtualTPS(60); //Sets the virtual TPS

	display.setUpdateCallback(cubeUpdate);
	display.setRenderCallback(cubeDisplay);

	display.createWindow(); //Create the window
}

static void setupCubeOpenGL() {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	manOGLUtil.setBackfaceCulling(GL_CCW);
}

static void loadCubeResources() {
	//Load Resources
	vao = objLoader.genVAOFromFile("./data/models/town.obj", &numIndicesToDraw);
	cubeShader = manShader.newFromGroup("./data/shaders/", "house");
	tex = textureUtil.createTextureFromFile("./data/texture/town.bmp", GL_LINEAR, GL_LINEAR);

	projMatrix = createProjectionMatrix(1.152f, 1.0f, 100.0f, (float) display.getWindowWidth() / (float) display.getWindowHeight());
	manShader.bindUniformMat4(cubeShader, "projectionMatrix", &projMatrix);

	modelMatrix = manMat4.createLeading(NULL, 1.0f);
	modelMatrix.data[3].z = -19.0f;
	modelMatrix.data[3].y = -3.8f;
	modelMatrix.data[3].x = -5.0f;
	manShader.bindUniformMat4(cubeShader, "modelMatrix", &modelMatrix);

	// Bind Texture Uniform
	glUseProgram(cubeShader->program);
	glUniform1i(glGetUniformLocation(cubeShader->program, "tex"), 0);
}

static Mat4 createProjectionMatrix(float verticalFovRad, float nearZClip, float farZClip, float aspectRatio) 
{
	float range = tan(verticalFovRad * 0.5f) * nearZClip;
	
	float Sx = (2.0f * nearZClip) / (range * aspectRatio + range * aspectRatio);
	float Sy = nearZClip / range;
	float Sz = -(farZClip + nearZClip) / (farZClip - nearZClip);
	float Pz = -(2.0f * farZClip * nearZClip) / (farZClip - nearZClip);

	return (manMat4.create(
		NULL,
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, Pz,
		0.0f, 0.0f, -1.0f, 0.0f));
}
