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

static GLuint vao;
static int numIndicesToDraw;
static Shader *cubeShader;
static Texture *tex;
Window* window;

Vec3* camPos;
Vec3* camRot;

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

	manMat.setMode(MATRIX_MODE_VIEW);
	manMat.push();
		manMat.translate(*camPos);
		manMat.rotate(camRot->z, manVec3.create(NULL, 0, 0, 1));
		manMat.rotate(camRot->y, manVec3.create(NULL, 0, 1, 0));
		manMat.rotate(camRot->x, manVec3.create(NULL, 1, 0, 0));

		manMat.setMode(MATRIX_MODE_MODEL);
		manShader.bind(cubeShader);
		manTex.bind(tex, 0);
			manMat.push();
				glBindVertexArray(vao);
					manShader.bindUniformMat4(cubeShader, "projectionMatrix", manMat.peekStack(MATRIX_MODE_PROJECTION));
					manShader.bindUniformMat4(cubeShader, "viewMatrix", manMat.peekStack(MATRIX_MODE_VIEW));
					manShader.bindUniformMat4(cubeShader, "modelMatrix", manMat.peekStack(MATRIX_MODE_MODEL));
					glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			manMat.pop();
		manTex.unbind(tex);
		manShader.unbind();
	manMat.setMode(MATRIX_MODE_VIEW);
	manMat.pop();
}

static void cubeUpdate(uint32_t delta) {
	glViewport(0, 0, manWin.getWidth(window), manWin.getHeight(window));

	if(manMouse.isDown(window, MOUSE_BUTTON_LEFT)) {
		camPos->x += manMouse.getDY(window)*sin(camRot->y)/-100;
		camPos->z += manMouse.getDY(window)*cos(camRot->y)/-100;

		camPos->y += manMouse.getDX(window)*sin(camRot->x)/-100;
	} else if(manMouse.isDown(window, MOUSE_BUTTON_RIGHT)) {
		camRot->x -= manMouse.getDY(window)/100;
		camRot->y += manMouse.getDX(window)/100;
	}
}

static void setupCubeDisplay() {
	manWin.setSize(window, 800, 600);
	manWin.centerWindow(window);
	manWin.openWindow(window); //Create the window
}

static void setupCubeOpenGL() {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE);
	manOGLUtil.setBackfaceCulling(GL_CCW);
}

static void loadCubeResources() {
	//Load Resources
	vao = objLoader.genVAOFromFile("./data/models/house2.obj", &numIndicesToDraw);
	cubeShader = manShader.newFromGroup("./data/shaders/", "house");
	tex = textureUtil.createTextureFromFile("./data/texture/house2.bmp", GL_LINEAR, GL_LINEAR);

	manMat.setMode(MATRIX_MODE_PROJECTION);
	manMat.pushPerspective(1.152f, (float)manWin.getWidth(window)/(float)manWin.getHeight(window), 0.1, 100);
	manMat.setMode(MATRIX_MODE_VIEW);
	manMat.pushIdentity();
	manMat.setMode(MATRIX_MODE_MODEL);
	manMat.pushIdentity();

	camPos = malloc(sizeof(Vec3));
	camRot = malloc(sizeof(Vec3));
	manVec3.create(camPos, 0,0,0);
	manVec3.create(camRot, 0,0,0);

	//Bind Texture Uniform
	glUseProgram(cubeShader->program);
	glUniform1i(glGetUniformLocation(cubeShader->program, "tex"), 0);
}
