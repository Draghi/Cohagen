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
#include "physics/Particle.h"
#include "physics/GravityForceGenerator.h"
#include "physics/ParticleForceRegistry.h"
#include "physics/AnchoredSpringForceGenerator.h"

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
	Shader *shaderPassThru;
	Texture* texTown;
	MatrixManager* manMat;

	Vec3* camPos;
	Vec3* camRot;

	// Physics
	Particle *particle;
	ParticleForceRegistry *particleForceRegistry;
	GravityForceGenerator *gravityFG;
	AnchoredSpringForceGenerator *anchoredSpringFG;
	Vec3 *gravity;
	Vec3 *anchor;
} InternalData;

void onCreate(GameLoop* self) {
	self->extraData = malloc(sizeof(InternalData));
	((InternalData*)self->extraData)->camPos = malloc(sizeof(Vec3));
	((InternalData*)self->extraData)->camRot = malloc(sizeof(Vec3));

	((InternalData *)self->extraData)->particle = manParticle.new();
	((InternalData *)self->extraData)->particleForceRegistry = manForceRegistry.new();
	((InternalData *)self->extraData)->gravity = malloc(sizeof(Vec3));
	((InternalData *)self->extraData)->anchor = malloc(sizeof(Vec3));
	((InternalData *)self->extraData)->manMat = manMatMan.new();
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

	data->vaoTown    	= objLoader.genVAOFromFile("./data/models/town.obj", &data->iCountTown);
	data->shaderHouse 	= manShader.newFromGroup("./data/shaders/", "house");
	data->texTown    	= textureUtil.createTextureFromFile("./data/texture/town.bmp", GL_LINEAR, GL_LINEAR);

	data->vaoSphere 		= objLoader.genVAOFromFile("./data/models/sphere.obj", &data->iCountSphere);
	data->shaderPassThru	= manShader.newFromGroup("./data/shaders/", "passThru");

	glUseProgram(data->shaderHouse->program);
	glUniform1i(glGetUniformLocation(data->shaderHouse->program, "tex"), 0);
}

void onInitMisc(GameLoop* self) {
	InternalData* data = self->extraData;

	manVec3.create(data->camPos, 0,0,0);
	manVec3.create(data->camRot, 0,0,0);

	manMatMan.setMode(data->manMat, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(data->manMat, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.1, 100);
	manMatMan.setMode(data->manMat, MATRIX_MODE_VIEW);
	manMatMan.pushIdentity(data->manMat);
	manMatMan.setMode(data->manMat, MATRIX_MODE_MODEL);
	manMatMan.pushIdentity(data->manMat);

	// Create and register GravityForceGenerator
	manVec3.create(data->gravity, 0.0f, -0.001f, 0.0f);
	((InternalData *)self->extraData)->gravityFG = manGravityForceGenerator.new(data->gravity);
	manForceRegistry.add(data->particleForceRegistry, data->particle, &(data->gravityFG->forceGenerator));

	// Create and register AnchoredSpringForceGenerator
	manVec3.create(data->anchor, 0.0f, 0.0f, 0.0f);
	((InternalData *)self->extraData)->anchoredSpringFG = manAnchoredSpringForceGenerator.new(data->anchor, 1.0f, 15.0f);
	// manForceRegistry.add(data->particleForceRegistry, data->particle, &(data->anchoredSpringFG->forceGenerator));
}

void onUpdate(GameLoop* self, float tickDelta) {
	InternalData* data = self->extraData;

	glViewport(0, 0, manWin.getFramebufferWidth(self->primaryWindow), manWin.getFramebufferHeight(self->primaryWindow));

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

	printf("%f %f %f\n", manParticle.getPosition(data->particle, NULL).x, manParticle.getPosition(data->particle, NULL).y, manParticle.getPosition(data->particle, NULL).z);

	// Update forces acting on particle
	manForceRegistry.updateForces(data->particleForceRegistry, tickDelta);

	// Update particle
	manParticle.integrate(data->particle, tickDelta);
}

void onRender(GameLoop* self, float frameDelta) {
	InternalData* data = self->extraData;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manMatMan.setMode(data->manMat, MATRIX_MODE_VIEW);
	manMatMan.push(data->manMat);
		manMatMan.translate(data->manMat, *data->camPos);
		manMatMan.rotate(data->manMat, data->camRot->z, manVec3.create(NULL, 0, 0, 1));
		manMatMan.rotate(data->manMat, data->camRot->y, manVec3.create(NULL, 0, 1, 0));
		manMatMan.rotate(data->manMat, data->camRot->x, manVec3.create(NULL, 1, 0, 0));

		manMatMan.setMode(data->manMat, MATRIX_MODE_MODEL);
			manShader.bind(data->shaderHouse);
			manTex.bind(data->texTown, 0);
				manMatMan.push(data->manMat);
					glBindVertexArray(data->vaoTown);
						manShader.bindUniformMat4(data->shaderHouse, "projectionMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_PROJECTION));
						manShader.bindUniformMat4(data->shaderHouse, "viewMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_VIEW));
						manShader.bindUniformMat4(data->shaderHouse, "modelMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_MODEL));
						glDrawElements(GL_TRIANGLES, data->iCountTown, GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);
				manMatMan.pop(data->manMat);
			manTex.unbind(data->texTown);
			manShader.unbind();

			manShader.bind(data->shaderPassThru);
				manMatMan.push(data->manMat);
					manMatMan.translate(data->manMat, data->particle->position);
					glBindVertexArray(data->vaoSphere);
						manShader.bindUniformMat4(data->shaderPassThru, "projectionMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_PROJECTION));
						manShader.bindUniformMat4(data->shaderPassThru, "viewMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_VIEW));
						manShader.bindUniformMat4(data->shaderPassThru, "modelMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_MODEL));
						glDrawElements(GL_TRIANGLES, data->iCountSphere, GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);
				manMatMan.pop(data->manMat);
			manShader.unbind();
		manMatMan.setMode(data->manMat, MATRIX_MODE_VIEW);
	manMatMan.pop(data->manMat);
}

void onClose(GameLoop* self) {
	InternalData* data = self->extraData;

	manTex.delete(data->texTown);
	free(data->shaderHouse);
	free(data->camPos);
	free(data->camRot);
	manParticle.delete(data->particle);
	manForceRegistry.delete(data->particleForceRegistry);
	manGravityForceGenerator.delete(data->gravityFG);
	free(data->gravity);
	free(data->anchor);
	manAnchoredSpringForceGenerator.delete(data->anchoredSpringFG);
}

void onDestroy(GameLoop* self) {
	free(self->extraData);
}