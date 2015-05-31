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
#include "render/MatrixManager.h"
#include "physics/Particle.h"
#include "physics/GravityForceGenerator.h"
#include "physics/ParticleForceRegistry.h"
#include "physics/AnchoredSpringForceGenerator.h"
#include "render/Skybox.h"

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
	VAO *vaoTown;
	int iCountTown;
	VAO *vaoSphere;
	int iCountSphere;
	Shader* shaderHouse;
	Shader *shaderPassThru;
	Texture* texTown;
	MatrixManager* manMat;

	Skybox *skybox;
	Shader *skyboxShader;

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

static void onCreate(GameLoop* self) {
	self->extraData = malloc(sizeof(InternalData));
	((InternalData*)self->extraData)->camPos = malloc(sizeof(Vec3));
	((InternalData*)self->extraData)->camRot = malloc(sizeof(Vec3));

	((InternalData *)self->extraData)->particle = manParticle.new(NULL, NULL, NULL, NULL);
	((InternalData *)self->extraData)->particleForceRegistry = manForceRegistry.new();
	((InternalData *)self->extraData)->gravity = malloc(sizeof(Vec3));
	((InternalData *)self->extraData)->anchor = malloc(sizeof(Vec3));
	((InternalData *)self->extraData)->manMat = manMatMan.new();
}

static void onInitWindow(GameLoop* self) {
	manWin.setSize(self->primaryWindow, 800, 600);
	manWin.centerWindow(self->primaryWindow);
}

static void onInitOpenGL(GameLoop* self) {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE);
	manOGLUtil.setBackfaceCulling(GL_CCW);

	InternalData* data = self->extraData;

	data->vaoTown    	= objLoader.genVAOFromFile("./data/models/town.obj", 0, 1, 2);
	data->shaderHouse 	= manShader.newFromGroup("./data/shaders/", "house");
	data->texTown    	= textureUtil.createTextureFromFile("./data/texture/town.bmp", GL_LINEAR, GL_LINEAR);

	data->vaoSphere 		= objLoader.genVAOFromFile("./data/models/sphere.obj", 0, 1, 2);
	data->shaderPassThru	= manShader.newFromGroup("./data/shaders/", "passThru");

	manShader.bindUniformInt(data->shaderHouse, "tex", 0);

	((InternalData *)self->extraData)->skybox = manSkybox.newFromGroup("./data/texture/", "purplenebula");
	data->skyboxShader = manShader.newFromGroup("./data/shaders/", "skybox");	
}

static void onInitMisc(GameLoop* self) {
	InternalData* data = self->extraData;

	manVec3.create(data->camPos, 0,0,0);
	manVec3.create(data->camRot, 0,0,0);

	manMatMan.setMode(data->manMat, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(data->manMat, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.1, 100);
	manMatMan.setMode(data->manMat, MATRIX_MODE_VIEW);
	manMatMan.pushIdentity(data->manMat);
	manMatMan.setMode(data->manMat, MATRIX_MODE_MODEL);
	manMatMan.pushIdentity(data->manMat);

	manParticle.setDamping(data->particle, 0.8f);

	// Create and register GravityForceGenerator
	manVec3.create(data->gravity, 0.0f, -9.8f, 0.0f);
	((InternalData *)self->extraData)->gravityFG = manGravityForceGenerator.new(data->gravity);
	manForceRegistry.add(data->particleForceRegistry, data->particle, &(data->gravityFG->forceGenerator));

	// Create and register AnchoredSpringForceGenerator
	manVec3.create(data->anchor, 0.0f, 0.0f, 0.0f);
	((InternalData *)self->extraData)->anchoredSpringFG = manAnchoredSpringForceGenerator.new(data->anchor, 10.00f, 0.8f, 3.0f);
	manForceRegistry.add(data->particleForceRegistry, data->particle, &(data->anchoredSpringFG->forceGenerator));
}

static void onUpdate(GameLoop* self, float tickDelta) {
	InternalData* data = self->extraData;

	glViewport(0, 0, manWin.getFramebufferWidth(self->primaryWindow), manWin.getFramebufferHeight(self->primaryWindow));

	float rate = 5.0f;

	if (manKeyboard.isDown(self->primaryWindow, KEY_LSHIFT)) {
		rate = 1.0f;
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_W)) {
		data->camPos->x +=  rate*cos(data->camRot->y-1.57079632679)*tickDelta;
		data->camPos->z +=  rate*sin(data->camRot->y-1.57079632679)*tickDelta;
		data->camPos->y += -rate*sin(data->camRot->x)*tickDelta;
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_S)) {
		data->camPos->x +=  rate*cos(data->camRot->y+1.57079632679)*tickDelta;
		data->camPos->z +=  rate*sin(data->camRot->y+1.57079632679)*tickDelta;
		data->camPos->y +=  rate*sin(data->camRot->x)*tickDelta;
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_A)) {
		data->camPos->x += -rate*cos(data->camRot->y)*tickDelta;
		data->camPos->z += -rate*sin(data->camRot->y)*tickDelta;
	}

	if(manKeyboard.isDown(self->primaryWindow, KEY_D)) {
		data->camPos->x += rate*cos(data->camRot->y)*tickDelta;
		data->camPos->z += rate*sin(data->camRot->y)*tickDelta;
	}

	if(manMouse.isDown(self->primaryWindow, MOUSE_BUTTON_RIGHT)) {
		data->camRot->x -= manMouse.getDY(self->primaryWindow)/100;
		data->camRot->y += manMouse.getDX(self->primaryWindow)/100;
	}

	// data->anchor->x = -data->camPos->x;
	// data->anchor->y = -data->camPos->y;
	// data->anchor->z = -data->camPos->z;

	// Update forces acting on particle
	manForceRegistry.updateForces(data->particleForceRegistry, tickDelta);

	// Update particle
	manParticle.integrate(data->particle, tickDelta);
}

static void onRender(GameLoop* self, float frameDelta) {
	InternalData* data = self->extraData;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manMatMan.setMode(data->manMat, MATRIX_MODE_VIEW);
	manMatMan.push(data->manMat);
		manMatMan.rotate(data->manMat, data->camRot->x, manVec3.create(NULL, 1, 0, 0));
		manMatMan.rotate(data->manMat, data->camRot->y, manVec3.create(NULL, 0, 1, 0));
		manMatMan.rotate(data->manMat, data->camRot->z, manVec3.create(NULL, 0, 0, 1));
		manMatMan.translate(data->manMat, *data->camPos);

		manShader.bind(data->skyboxShader);
			manShader.bindUniformMat4(data->skyboxShader, "projectionMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_PROJECTION));
			manShader.bindUniformMat4(data->skyboxShader, "viewMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_VIEW));
			manSkybox.draw(data->skybox, data->skyboxShader, "cubeTexture");			
		manShader.unbind();

		manMatMan.setMode(data->manMat, MATRIX_MODE_MODEL);
			manShader.bind(data->shaderHouse);
				manTex.bind(data->texTown, GL_TEXTURE_2D, 0);
					manMatMan.push(data->manMat);
						manVAO.bind(data->vaoTown);
							manShader.bindUniformMat4(data->shaderHouse, "projectionMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_PROJECTION));
							manShader.bindUniformMat4(data->shaderHouse, "viewMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_VIEW));
							manShader.bindUniformMat4(data->shaderHouse, "modelMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_MODEL));

							manVAO.draw(data->vaoTown);
						manVAO.unbind();
					manMatMan.pop(data->manMat);
				manTex.unbind(data->texTown, GL_TEXTURE_2D);
			manShader.unbind();

			manMatMan.push(data->manMat);
				manShader.bind(data->shaderPassThru);
					manMatMan.translate(data->manMat, *data->particle->position);
					manVAO.bind(data->vaoSphere);
						manShader.bindUniformMat4(data->shaderPassThru, "projectionMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_PROJECTION));
						manShader.bindUniformMat4(data->shaderPassThru, "viewMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_VIEW));
						manShader.bindUniformMat4(data->shaderPassThru, "modelMatrix", manMatMan.peekStack(data->manMat, MATRIX_MODE_MODEL));

						manVAO.draw(data->vaoSphere);
					manVAO.unbind();
				manShader.unbind();
			manMatMan.pop(data->manMat);
		manMatMan.setMode(data->manMat, MATRIX_MODE_VIEW);
	manMatMan.pop(data->manMat);
}

static void onClose(GameLoop* self) {
	InternalData* data = self->extraData;

	// manTex.delete(data->texTown);
	free(data->shaderHouse);
	free(data->camPos);
	free(data->camRot);
	manParticle.delete(data->particle);
	manForceRegistry.delete(data->particleForceRegistry);
	manGravityForceGenerator.delete(data->gravityFG);
	free(data->gravity);
	free(data->anchor);
	manAnchoredSpringForceGenerator.delete(data->anchoredSpringFG);

	manVAO.delete(data->vaoTown);
	manVAO.delete(data->vaoSphere);
}

static void onDestroy(GameLoop* self) {
	free(self->extraData);
}
