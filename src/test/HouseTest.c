#include "HouseTest.h"

#include <stdio.h>
#include <math.h>

#include "render/MatrixManager.h"
#include "lib/ogl.h"
#include "glfw/Display.h"
#include "glfw/Keyboard.h"
#include "glfw/Mouse.h"
#include "util/ObjLoader.h"
#include "util/OGLUtil.h"
#include "util/TextureUtil.h"
#include "gl/Shader.h"
#include "gl/Textures.h"
#include "math/Mat4.h"

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
Window* window;

void runHouseTest() {
	window = manWin.new();
	setupCubeDisplay();
	setupCubeOpenGL();
	loadCubeResources();
	while(manWin.isOpen(window)) {
		cubeUpdate(1);
		cubeDisplay();
		manWin.update(window);
		manWin.swapBuffers(window);
	}
}

static void cubeDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE);

	manTex.bind(tex, 0);
		manMat.push();
			glBindVertexArray(vao);
				manShader.bindUniformMat4(cubeShader, "modelMatrix", manMat.peek());
				manShader.bind(cubeShader);
					glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, 0);
				manShader.unbind();
			glBindVertexArray(0);
		manMat.pop();
	manTex.unbind(tex);

	glDisable(GL_TEXTURE);
}

static void cubeUpdate(uint32_t delta) {
	glViewport(0, 0, manWin.getWidth(window), manWin.getHeight(window));

	if(manMouse.isDown(window, MOUSE_BUTTON_LEFT)) {
		manMat.translate(manVec3.create(NULL, manMouse.getDX(window)/100, 0, manMouse.getDY(window)/100));
	}

	if(manMouse.isDown(window, MOUSE_BUTTON_RIGHT)) {
		manMat.rotate(manMouse.getDY(window)/100,manVec3.create(NULL, 1,0,0));
		manMat.rotate(manMouse.getDX(window)/100,manVec3.create(NULL, 0,1,0));
	}

	manShader.bindUniformMat4(cubeShader, "modelMatrix", &modelMatrix);
}

static void setupCubeDisplay() {
	manWin.setSize(window, 800, 600);
	manWin.openWindow(window); //Create the window
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
	vao = objLoader.genVAOFromFile("./data/models/house2.obj", &numIndicesToDraw);
	cubeShader = manShader.newFromGroup("./data/shaders/", "house");
	tex = textureUtil.createTextureFromFile("./data/texture/house2.bmp", GL_LINEAR, GL_LINEAR);

	projMatrix = createProjectionMatrix(1.152f, 1.0f, 100.0f, (float) manWin.getWidth(window) /(float) manWin.getHeight(window));
	manShader.bindUniformMat4(cubeShader, "projectionMatrix", &projMatrix);

	manMat.setMode(MATRIX_MODE_MODEL);
	manMat.pushIdentity();

	//Bind Texture Uniform
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
