#include "Tests.h"

#include "engine/GameLoop.h"
#include "glfw/Display.h"
#include "gl/Shader.h"
#include "gl/ShaderLoader.h"
#include "gl/Textures.h"
#include "gl/VAO.h"
#include "math/Vec3.h"
#include "util/ObjLoader.h"
#include "util/TextureUtil.h"
#include "util/OGLUtil.h"
#include "engine/MatrixManager.h"

#include <stdio.h>
#include <math.h>

GameLoop* gameloop;

void onCreate(GameLoop* self);
void onInitWindow(GameLoop* self);
void onInitOpenGL(GameLoop* self);
void onInitMisc(GameLoop* self);
void onUpdate(GameLoop* self, float tickDelta);
void onRender(GameLoop* self, float frameDelta);
void onClose(GameLoop* self);
void onDestroy(GameLoop* self);

void runGameLoopTest() {
	gameloop = manGameLoop.new(&onCreate, &onInitWindow, &onInitOpenGL, &onInitMisc, &onUpdate, &onRender, &onClose, &onDestroy, 1/120.0, 1/60.0);
	manGameLoop.enterGameLoop(gameloop);
	manGameLoop.delete(gameloop);
}

VAO *vao;
Shader* shader;
Texture* tex;
Vec3* camPos;
Vec3* camRot;
MatrixManager* manMat;

void onCreate(GameLoop* self) {

	camPos = malloc(sizeof(Vec3));
	camRot = malloc(sizeof(Vec3));
	manMat = manMatMan.new();
}

void onInitWindow(GameLoop* self) {
	manWin.setSize(self->primaryWindow, 800, 600);
	manWin.centerWindow(self->primaryWindow);
}

void onInitOpenGL(GameLoop* self) {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE);
	manOGLUtil.setBackfaceCulling(GL_CCW);

	vao    = objLoader.genVAOFromFile("./data/models/town.obj");
	shader = manShader.newFromGroup("./data/shaders/", "house");
	tex    = textureUtil.createTextureFromFile("./data/texture/town.bmp", GL_LINEAR, GL_LINEAR);

	glUseProgram(shader->program);
	glUniform1i(glGetUniformLocation(shader->program, "tex"), 0);
}

void onInitMisc(GameLoop* self) {
	manVec3.create(camPos, 0,0,0);
	manVec3.create(camRot, 0,0,0);

	manMatMan.setMode(manMat, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(manMat, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.1, 100);
	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.pushIdentity(manMat);
	manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
	manMatMan.pushIdentity(manMat);
}

void onUpdate(GameLoop* self, float tickDelta) {
	glViewport(0, 0, manWin.getFramebufferWidth(self->primaryWindow), manWin.getFramebufferHeight(self->primaryWindow));

	float rate = 0.2f;

	if (manKeyboard.isDown(self->primaryWindow, KEY_LSHIFT)) {
		rate = 0.005f;
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_W)) {
		camPos->x +=  rate*cos(camRot->y-1.57079632679);
		camPos->z +=  rate*sin(camRot->y-1.57079632679);
		camPos->y += -rate*sin(camRot->x);
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_S)) {
		camPos->x +=  rate*cos(camRot->y+1.57079632679);
		camPos->z +=  rate*sin(camRot->y+1.57079632679);
		camPos->y +=  rate*sin(camRot->x);
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_A)) {
		camPos->x += -rate*cos(camRot->y);
		camPos->z += -rate*sin(camRot->y);
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_D)) {
		camPos->x += rate*cos(camRot->y);
		camPos->z += rate*sin(camRot->y);
	}

	if(manMouse.isDown(self->primaryWindow, MOUSE_BUTTON_RIGHT)) {
		camRot->x -= manMouse.getDY(self->primaryWindow)/100;
		camRot->y += manMouse.getDX(self->primaryWindow)/100;
	}
}

void onRender(GameLoop* self, float frameDelta) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.push(manMat);
		manMatMan.rotate(manMat, camRot->z, manVec3.create(NULL, 0, 0, 1));
		manMatMan.rotate(manMat, camRot->y, manVec3.create(NULL, 0, 1, 0));
		manMatMan.rotate(manMat, camRot->x, manVec3.create(NULL, 1, 0, 0));
		manMatMan.translate(manMat, *camPos);

		manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
		manShader.bind(shader);
		manTex.bind(tex, GL_TEXTURE_2D, 0);
			manMatMan.push(manMat);
				manVAO.bind(vao);
					manShader.bindUniformMat4(shader, "projectionMatrix", manMatMan.peekStack(manMat, MATRIX_MODE_PROJECTION));
					manShader.bindUniformMat4(shader, "viewMatrix", manMatMan.peekStack(manMat, MATRIX_MODE_VIEW));
					manShader.bindUniformMat4(shader, "modelMatrix", manMatMan.peekStack(manMat, MATRIX_MODE_MODEL));
					manVAO.draw(vao);
				manVAO.unbind();
			manMatMan.pop(manMat);
		manTex.unbind(tex, GL_TEXTURE_2D);
		manShader.unbind();
	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.pop(manMat);
}

void onClose(GameLoop* self) {
	free(tex);
	free(shader);
}

void onDestroy(GameLoop* self) {
	manMatMan.delete(manMat);
	free(manMat);
	free(camPos);
	free(camRot);
	manVAO.delete(vao);
}
