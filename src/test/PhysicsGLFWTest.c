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

static void onCreate(GameLoop* self);
static void onInitWindow(GameLoop* self);
static void onInitOpenGL(GameLoop* self);
static void onInitMisc(GameLoop* self);
static void onUpdate(GameLoop* self, float tickDelta);
static void onRender(GameLoop* self, float frameDelta);
static void onClose(GameLoop* self);
static void onDestroy(GameLoop* self);

void runPhysicsGLFWTest() {
	gameloop = manGameLoop.new(&onCreate, &onInitWindow, &onInitOpenGL, &onInitMisc, &onUpdate, &onRender, &onClose, &onDestroy, 1/120.0, 1/60.0);
	manGameLoop.enterGameLoop(gameloop);
	manGameLoop.delete(gameloop);
}

typedef struct InternalData_s {
	int vaoTown;
	int iCountTown;
	int vaoSphere;
	int iCountSphere;
	Shader* shaderHouse;
	Texture* texTown;
	Vec3* camPos;
	Vec3* camRot;
} InternalData;

void onCreate(GameLoop* self) {
	self->extraData = malloc(sizeof(InternalData));
	((InternalData*)self->extraData)->camPos = malloc(sizeof(Vec3));
	((InternalData*)self->extraData)->camRot = malloc(sizeof(Vec3));
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

	InternalData* data = self->extraData;

	data->vaoTown    = objLoader.genVAOFromFile("./data/models/town.obj", &data->iCountTown);
	data->shaderHouse = manShader.newFromGroup("./data/shaders/", "house");
	data->texTown    = textureUtil.createTextureFromFile("./data/texture/town.bmp", GL_LINEAR, GL_LINEAR);

	glUseProgram(data->shaderHouse->program);
	glUniform1i(glGetUniformLocation(data->shaderHouse->program, "tex"), 0);
}

void onInitMisc(GameLoop* self) {
	InternalData* data = self->extraData;

	manVec3.create(data->camPos, 0,0,0);
	manVec3.create(data->camRot, 0,0,0);

	manMat.setMode(MATRIX_MODE_PROJECTION);
	manMat.pushPerspective(1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.1, 100);
	manMat.setMode(MATRIX_MODE_VIEW);
	manMat.pushIdentity();
	manMat.setMode(MATRIX_MODE_MODEL);
	manMat.pushIdentity();
}

void onUpdate(GameLoop* self, float tickDelta) {
	InternalData* data = self->extraData;

	glViewport(0, 0, manWin.getWidth(self->primaryWindow), manWin.getHeight(self->primaryWindow));

	float rate = 0.2f;

	if (manKeyboard.isDown(self->primaryWindow, KEY_LSHIFT)) {
		rate = 0.005f;
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_W)) {
		data->camPos->x += -rate*sin(data->camRot->y);
		data->camPos->z += -rate*cos(data->camRot->y);
		data->camPos->y +=  rate*sin(data->camRot->x);
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_S)) {
		data->camPos->x +=  rate*sin(data->camRot->y);
		data->camPos->z +=  rate*cos(data->camRot->y);
		data->camPos->y += -rate*sin(data->camRot->x);
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_A)) {
		data->camPos->x += rate*sin(data->camRot->y-1.57079632679);
		data->camPos->z += rate*cos(data->camRot->y-1.57079632679);
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_D)) {
		data->camPos->x += rate*sin(data->camRot->y+1.57079632679);
		data->camPos->z += rate*cos(data->camRot->y+1.57079632679);
	}

	if(manMouse.isDown(self->primaryWindow, MOUSE_BUTTON_RIGHT)) {
		data->camRot->x -= manMouse.getDY(self->primaryWindow)/100;
		data->camRot->y += manMouse.getDX(self->primaryWindow)/100;
	}
}

void onRender(GameLoop* self, float frameDelta) {
	InternalData* data = self->extraData;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manMat.setMode(MATRIX_MODE_VIEW);
	manMat.push();
		manMat.translate(*data->camPos);
		manMat.rotate(data->camRot->z, manVec3.create(NULL, 0, 0, 1));
		manMat.rotate(data->camRot->y, manVec3.create(NULL, 0, 1, 0));
		manMat.rotate(data->camRot->x, manVec3.create(NULL, 1, 0, 0));

		manMat.setMode(MATRIX_MODE_MODEL);
		manShader.bind(data->shaderHouse);
		manTex.bind(data->texTown, 0);
			manMat.push();
				glBindVertexArray(data->vaoTown);
					manShader.bindUniformMat4(data->shaderHouse, "projectionMatrix", manMat.peekStack(MATRIX_MODE_PROJECTION));
					manShader.bindUniformMat4(data->shaderHouse, "viewMatrix", manMat.peekStack(MATRIX_MODE_VIEW));
					manShader.bindUniformMat4(data->shaderHouse, "modelMatrix", manMat.peekStack(MATRIX_MODE_MODEL));
					glDrawElements(GL_TRIANGLES, data->iCountTown, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			manMat.pop();
		manTex.unbind(data->texTown);
		manShader.unbind();
	manMat.setMode(MATRIX_MODE_VIEW);
	manMat.pop();
}

void onClose(GameLoop* self) {
	InternalData* data = self->extraData;

	manTex.delete(data->texTown);
	free(data->shaderHouse);
	free(data->camPos);
	free(data->camRot);
}

void onDestroy(GameLoop* self) {
	free(self->extraData);
}
