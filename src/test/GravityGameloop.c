#include "Tests.h"

#include "engine/GameLoop.h"
#include "render/Skybox.h"
#include "render/MatrixManager.h"
#include "render/RenderObject.h"
#include "render/Renderer.h"
#include "col/CollisionDetection.h"
#include "col/CollisionResponse.h"
#include "physics/AnchoredGravityForceGenerator.h"
#include "physics/ParticleForceRegistry.h"
#include "gl/ShaderLoader.h"
#include "util/OGLUtil.h"
#include "util/ObjLoader.h"

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

void runGravity() {
	gameloop = manGameLoop.new(&onCreate, &onInitWindow, &onInitOpenGL, &onInitMisc, &onUpdate, &onRender, &onClose, &onDestroy, 1/120.0, 1/60.0);
	manGameLoop.enterGameLoop(gameloop);
	manGameLoop.delete(gameloop);
}

typedef struct NewtonsCradleData_s {
	//Particle Related
	int particleCount;
	Particle** particles;
	ParticleForceRegistry* pfRegist;
	AnchoredGravityForceGenerator* gfGen;

	//Collision Related
	PhysicsCollider* baseCollider;
	PhysicsCollider** cubes;

	//Rendering related
	MatrixManager *manMat;
	Vec3 *camPos;
	Vec3 *camRot;

	//Cube Rendering
	Shader *cubeShader;
	RenderObject **cubeModel;

	//Skybox Rendering
	Skybox *skybox;
	Shader *skyboxShader;

	//Village Rendering
	RenderObject* villageModel;
	Shader *villageShader;

} NewtonsCradleData;

static void onCreate(GameLoop* self) {
	self->extraData = malloc(sizeof(NewtonsCradleData));
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	data->particleCount = 20;
	data->particles = malloc(sizeof(Particle*)*data->particleCount);
	data->cubes = malloc(sizeof(PhysicsCollider*)*data->particleCount);
	data->cubeModel = malloc(sizeof(RenderObject*)*data->particleCount);

	data->camPos = malloc(sizeof(Vec3));
	data->camRot = malloc(sizeof(Vec3));
}

static void onInitWindow(GameLoop* self) {
	manWin.setSize(self->primaryWindow, 800, 600);
	manWin.centerWindow(self->primaryWindow);
}

static void onInitOpenGL(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE);
	manOGLUtil.setBackfaceCulling(GL_CCW);

	data->cubeShader = manShader.newFromGroup("./data/shaders/", "passThruAlt");

	/** @todo: Add method to shader manager **/
	glUseProgram(data->cubeShader->program);
	glUniform1i(glGetUniformLocation(data->cubeShader->program, "tex"), 0);

	data->skybox = manSkybox.new("./data/texture/purplenebula_front.bmp", "./data/texture/purplenebula_back.bmp",
						         "./data/texture/purplenebula_top.bmp",   "./data/texture/purplenebula_top.bmp",
							     "./data/texture/purplenebula_left.bmp",  "./data/texture/purplenebula_right.bmp");
	data->skyboxShader = manShader.newFromGroup("./data/shaders/", "skybox");

	data->villageShader  = manShader.newFromGroup("./data/shaders/", "houseAlt");
	data->villageModel = manRenderObj.new(NULL, NULL, NULL);
	manRenderObj.setModel(data->villageModel, objLoader.genVAOFromFile("./data/models/town.obj"));
	manRenderObj.addTexture(data->villageModel, textureUtil.createTextureFromFile("./data/texture/town.bmp", GL_LINEAR, GL_LINEAR));
	data->villageModel->rotation->y = 0.785398163;
}

static void onInitMisc(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	data->baseCollider = objLoader.loadCollisionMesh("./data/models/meteor.col.obj", NULL, NULL, NULL, NULL);

	data->pfRegist = manForceRegistry.new();
	Particle* gravParticle = manParticle.new(NULL, NULL, NULL, NULL);
	manParticle.setMass(gravParticle, 10618583169100);
	manParticle.setPositionXYZ(gravParticle, 0, 10, 0);
	data->gfGen = manAnchoredGravityForceGenerator.new(gravParticle);

	VAO* cubeVAO = objLoader.genVAOFromFile("./data/models/meteor.obj");
	for(int i = 0; i<data->particleCount; i++) {
		data->particles[i] = manParticle.new(NULL, NULL, NULL, NULL);
		manParticle.setPositionXYZ(data->particles[i], 2*i*data->baseCollider->bPhase.radius - (data->particleCount*2*data->baseCollider->bPhase.radius)/2.0, 0, 0);
		manParticle.setMass(data->particles[i], 10618583169100);

		manForceRegistry.add(data->pfRegist, data->particles[i], &data->gfGen->forceGenerator);

		data->cubes[i] = manPhysCollider.new(data->particles[i]->position, NULL, NULL, data->particles[i]->velocity);
		data->cubes[i]->bPhase = data->baseCollider->bPhase;
		data->cubes[i]->nPhase = data->baseCollider->nPhase;

		data->cubeModel[i] = manRenderObj.new(data->cubes[i]->position, data->cubes[i]->rotation, data->cubes[i]->scale);
		manRenderObj.setModel(data->cubeModel[i], cubeVAO);
	}

	data->gfGen->gravityAnchor = data->particles[data->particleCount/2];


	manVec3.create(data->camPos, 0,0,0);
	manVec3.create(data->camRot, 0,0,0);


	data->manMat = manMatMan.new();
	manMatMan.setMode(data->manMat, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(data->manMat, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.1, 2000);
	manMatMan.setMode(data->manMat, MATRIX_MODE_VIEW);
	manMatMan.pushIdentity(data->manMat);
	manMatMan.setMode(data->manMat, MATRIX_MODE_MODEL);
	manMatMan.pushIdentity(data->manMat);
}

static void onClose(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	//free(data->cubeVAO);
	free(data->cubeShader);

	//free(data->villageVAO);
	free(data->villageShader);
	//free(data->villageTexture);

	/**@todo: Add skybox delete function. **/
	free(data->skybox->tex);
	free(data->skybox->vao);
	free(data->skybox);

	for(int i = 0; i<data->particleCount; i++) {
		manParticle.delete(data->particles[i]);

		free(data->cubes[i]->rotation);
		free(data->cubes[i]->scale);
		free(data->cubes[i]);
	}

	manMatMan.delete(data->manMat);
	free(data->manMat);

	manAnchoredGravityForceGenerator.delete(data->gfGen);
	manForceRegistry.delete(data->pfRegist);

	manColMesh.deleteSimpleMesh(data->baseCollider->nPhase.collider);

	free(data->baseCollider->position);
	free(data->baseCollider->rotation);
	free(data->baseCollider->scale);
	free(data->baseCollider->velocity);
	free(data->baseCollider);
}

static void onDestroy(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	free(data->camPos);
	free(data->camRot);
	free(data->particles);
	free(data->cubes);
	free(data);
}

////////////////
// Game-logic //
////////////////

static void update(float tickDelta, Window* window, Vec3* camPos, Vec3* camRot, ParticleForceRegistry* pfRegist, int particleCount, PhysicsCollider** cubes, Particle** particles) {
	float rate = 20.0f;

	if (manKeyboard.isDown(window, KEY_LSHIFT)) {
		rate = 0.5f;
	}

	if(manMouse.isDown(window, MOUSE_BUTTON_RIGHT)) {
		camRot->x -= manMouse.getDY(window)/100;
		camRot->y += manMouse.getDX(window)/100;
	}

	if(manKeyboard.isDown(window, KEY_W)) {
		camPos->x +=  rate*cos(camRot->y-1.57079632679)*tickDelta;
		camPos->z +=  rate*sin(camRot->y-1.57079632679)*tickDelta;
		camPos->y += -rate*sin(camRot->x)*tickDelta;
	}

	if(manKeyboard.isDown(window, KEY_S)) {
		camPos->x +=  rate*cos(camRot->y+1.57079632679)*tickDelta;
		camPos->z +=  rate*sin(camRot->y+1.57079632679)*tickDelta;
		camPos->y +=  rate*sin(camRot->x)*tickDelta;
	}

	if(manKeyboard.isDown(window, KEY_A)) {
		camPos->x += -rate*cos(camRot->y)*tickDelta;
		camPos->z += -rate*sin(camRot->y)*tickDelta;
	}

	if(manKeyboard.isDown(window, KEY_D)) {
		camPos->x += rate*cos(camRot->y)*tickDelta;
		camPos->z += rate*sin(camRot->y)*tickDelta;
	}

	if(manKeyboard.isDown(window, KEY_J)) {
		Vec3 force = {rate*10, 0, 0};
		manParticle.addForce(particles[0], &force);
	}

	if(manKeyboard.isDown(window, KEY_L)) {
		Vec3 force = {-rate*10, 0, 0};
		manParticle.addForce(particles[0], &force);
	}


	if(manKeyboard.isDown(window, KEY_SPACE)) {
		// Update forces acting on particle
		manForceRegistry.updateForces(pfRegist, tickDelta);

		// Update particle
		for(int i = 0; i < particleCount; i++)
			manParticle.integrate(particles[i], tickDelta);

		for(int i = 0; i < particleCount; i++) {
			for(int j = i+1; j < particleCount; j++) {
				if (manColDetection.checkStaticBroadphase(cubes[i], cubes[j])) {
					CollisionResult info = manColDetection.checkStaticNarrowphase(cubes[i], cubes[j]);
					if (info.isColliding) {
						//flag = true;
						Vec3 translation = manVec3.preMulScalar(info.distance/2, &info.axis);

						*cubes[i]->position = manVec3.sum(cubes[i]->position, &translation);
						translation = manVec3.invert(&translation);
						*cubes[j]->position = manVec3.sum(cubes[j]->position, &translation);

						momentumCollisionResponse(cubes[i]->velocity, cubes[j]->velocity, *cubes[i]->velocity, *cubes[j]->velocity, 1/particles[i]->inverseMass, 1/particles[j]->inverseMass);
					}
				}
			}
		}
	}
}

static void onUpdate(GameLoop* self, float tickDelta) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;
	update(tickDelta, self->primaryWindow, data->camPos, data->camRot, data->pfRegist, data->particleCount, data->cubes, data->particles);

	char title[80];

	sprintf(title, "Cohagen - FPS: %f, TPS: %f", self->fps, self->tps);

	manWin.setTitle(self->primaryWindow, title);
}

static void bindMatricies(Shader* sha, MatrixManager* mats) {
	manShader.bindUniformMat4(sha, "projectionMatrix", manMatMan.peekStack(mats, MATRIX_MODE_PROJECTION));
	manShader.bindUniformMat4(sha, "viewMatrix",       manMatMan.peekStack(mats, MATRIX_MODE_VIEW));
	manShader.bindUniformMat4(sha, "modelMatrix",      manMatMan.peekStack(mats, MATRIX_MODE_MODEL));
}

static void renderSkybox(MatrixManager* manMat, Shader* skyboxShader, Skybox* skybox) {
	manShader.bind(skyboxShader);
		bindMatricies(skyboxShader, manMat);
		manSkybox.draw(skybox, skyboxShader, "cubeTexture");
	manShader.unbind();
}

static const Vec3 xAxis = {1, 0, 0};
static const Vec3 yAxis = {0, 1, 0};
static const Vec3 zAxis = {0, 0, 1};

static void setupCamera(MatrixManager* mats, Vec3* camPos, Vec3* camRot) {
	manMatMan.rotate(mats, camRot->x, xAxis);
	manMatMan.rotate(mats, camRot->y, yAxis);
	manMatMan.rotate(mats, camRot->z, zAxis);
	manMatMan.translate(mats, *camPos);
}

static void render(float frameDelta, Window* window, MatrixManager* manMat, Vec3* camPos, Vec3* camRot, Skybox* skybox, Shader* skyboxShader, Shader* villageShader, RenderObject* villageModel, Shader* cubeShader, int particleCount, RenderObject** cubes) {
	glViewport(0, 0, manWin.getFramebufferWidth(window), manWin.getFramebufferHeight(window));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.push(manMat);
		setupCamera(manMat, camPos, camRot);

		renderSkybox(manMat, skyboxShader, skybox);

		manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
		manRenderer.renderModel(villageModel, villageShader, manMat);

		manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
		for(int i = 0; i <particleCount; i++)
			manRenderer.renderModel(cubes[i], cubeShader, manMat);

	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.pop(manMat);
}

static void onRender(GameLoop* self, float frameDelta) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;
	render(frameDelta, self->primaryWindow, data->manMat, data->camPos, data->camRot, data->skybox, data->skyboxShader, data->villageShader, data->villageModel, data->cubeShader, data->particleCount, data->cubeModel);
}
