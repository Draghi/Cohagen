#include "Tests.h"

#include "engine/GameLoop.h"
#include "engine/GameObjectRegistry.h"
#include "render/Skybox.h"
#include "render/MatrixManager.h"
#include "render/RenderObject.h"
#include "render/Renderer.h"
#include "col/CollisionDetection.h"
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
	AnchoredGravityForceGenerator* gfGen;

	//Rendering related
	Vec3 *camPos;
	Vec3 *camRot;

	//Skybox Rendering
	Skybox *skybox;
	Shader *skyboxShader;

	//Village Rendering
	RenderObject* villageModel;
	Shader *villageShader;

	GameObjectRegist* gameObjRegist;

} NewtonsCradleData;

static void onCreate(GameLoop* self) {
	self->extraData = malloc(sizeof(NewtonsCradleData));
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	data->camPos = malloc(sizeof(Vec3));
	data->camRot = malloc(sizeof(Vec3));
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
	glEnable(GL_CLIP_DISTANCE0);

	manOGLUtil.setBackfaceCulling(GL_CCW);
}

static void initSkybox(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	data->skybox = manSkybox.new("./data/texture/purplenebula_front.bmp", "./data/texture/purplenebula_back.bmp",
							         "./data/texture/purplenebula_top.bmp",   "./data/texture/purplenebula_top.bmp",
								     "./data/texture/purplenebula_left.bmp",  "./data/texture/purplenebula_right.bmp");
	data->skyboxShader = manShader.newFromGroup("./data/shaders/", "skybox");
}

static void initGravityGen(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	Particle* gravParticle = manParticle.new(NULL, NULL, NULL, NULL);
	manParticle.setMass(gravParticle, 10618583169100);
	manParticle.setPositionXYZ(gravParticle, 0, 10, 0);
	data->gfGen = manAnchoredGravityForceGenerator.new(gravParticle);
}

static void initVillage(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	data->villageShader  = manShader.newFromGroup("./data/shaders/", "texLogZ");
	manShader.bind(data->villageShader);
		manShader.bindUniformInt(data->villageShader, "tex", 0);
		manShader.bindUniformFloat(data->villageShader, "near", 0.001);
		manShader.bindUniformFloat(data->villageShader, "FCoef", 2.0/log(1000*0.001 + 1));
	manShader.unbind(data->villageShader);

	data->villageModel = manRenderObj.new(NULL, NULL, NULL);

	int posLoc = manShader.getAttribLocation(data->villageShader, "vPos");
	int normLoc = manShader.getAttribLocation(data->villageShader, "vNorm");
	int texLoc = manShader.getAttribLocation(data->villageShader, "vTex");
	VAO* vao = objLoader.genVAOFromFile("./data/models/town.obj", posLoc, normLoc, texLoc);
	manRenderObj.setModel(data->villageModel, vao);

	manRenderObj.addTexture(data->villageModel, textureUtil.createTextureFromFile("./data/texture/town.bmp", GL_LINEAR, GL_LINEAR));
	data->villageModel->rotation->y = 0.785398163;
}

static void initGameObjRegist(GameLoop* self) {
	MatrixManager* manMat = manMatMan.new();
	manMatMan.setMode(manMat, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(manMat, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.001, 1000);
	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.pushIdentity(manMat);
	manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
	manMatMan.pushIdentity(manMat);

	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	data->gameObjRegist = manGameObjRegist.new(manMat);
	manGameObjRegist.setShader(data->gameObjRegist, manShader.newFromGroup("./data/shaders/", "passThruAlt"));

	manShader.bind(data->gameObjRegist->currentShader);
		manShader.bindUniformInt(data->gameObjRegist->currentShader, "tex", 0);
	manShader.unbind(data->gameObjRegist->currentShader);
}

static void initWorld(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	PhysicsCollider* baseCollider = objLoader.loadCollisionMesh("./data/models/meteor.col.obj", NULL, NULL, NULL, NULL);

	VAO* cubeVAO = objLoader.genVAOFromFile("./data/models/meteor.obj", 0, 1, 2);
	RenderObject* baseRender = manRenderObj.new(NULL, NULL, NULL);
	manRenderObj.setModel(baseRender, cubeVAO);

	const int particleCount = 20;
	for(int i = 0; i<particleCount; i++) {
		GameObject* gameObject = manGameObj.new("Asteroid", NULL, true, true, NULL, NULL, NULL, NULL);

		manGameObj.setPositionXYZ(gameObject, 2*i*baseCollider->bPhase.radius - (particleCount*2*baseCollider->bPhase.radius)/2.0, 0, 0);
		manParticle.setMass(gameObject->particle, 10618583169100);
		manGameObj.setPhysicsCollider(gameObject, baseCollider);
		manGameObj.setModel(gameObject, baseRender);

		manGameObjRegist.add(data->gameObjRegist, gameObject);

		manGameObj.addForceGenerator(gameObject, &data->gfGen->forceGenerator);

		if (i==particleCount/2)
			data->gfGen->gravityAnchor = gameObject->particle;
	}
}

static void onInitMisc(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	manVec3.create(data->camPos, 0,0,0);
	manVec3.create(data->camRot, 0,0,0);

	initSkybox(self);
	initVillage(self);
	initGravityGen(self);
	initGameObjRegist(self);
	initWorld(self);
}

static void onClose(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	//free(data->cubeVAO);

	//free(data->villageVAO);
	free(data->villageShader);
	//free(data->villageTexture);

	/**@todo: Add skybox delete function. **/
	free(data->skybox->tex);
	free(data->skybox->vao);
	free(data->skybox);

	manAnchoredGravityForceGenerator.delete(data->gfGen);
}

static void onDestroy(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	free(data->camPos);
	free(data->camRot);
	free(data);
}

////////////////
// Game-logic //
////////////////

static void update(float tickDelta, Window* window, Vec3* camPos, Vec3* camRot, GameObjectRegist* gameObjRegist) {
	float rate = 20.0f;

	if (manKeyboard.isDown(window, KEY_LSHIFT)) {
		rate = 0.5f;
	}


	if (manKeyboard.isDown(window, KEY_LCONTROL)) {
		rate *= 0.01f;
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
		manGameObj.addForceXYZ(manGameObjRegist.getGameObject(gameObjRegist, 10), 1000000000000000*rate, 0,0);
	}

	if(manKeyboard.isDown(window, KEY_L)) {
		manGameObj.addForceXYZ(manGameObjRegist.getGameObject(gameObjRegist, 10), -1000000000000000*rate, 0,0);
	}

	manGameObjRegist.update(gameObjRegist, tickDelta);
}

static void onUpdate(GameLoop* self, float tickDelta) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;
	update(tickDelta, self->primaryWindow, data->camPos, data->camRot, data->gameObjRegist);

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

static void render(float frameDelta, Window* window, Vec3* camPos, Vec3* camRot, Skybox* skybox, Shader* skyboxShader, Shader* villageShader, RenderObject* villageModel, GameObjectRegist* gameObjRegist) {
	glViewport(0, 0, manWin.getFramebufferWidth(window), manWin.getFramebufferHeight(window));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MatrixManager* matMan = gameObjRegist->matMan;
	manMatMan.setMode(matMan, MATRIX_MODE_VIEW);
	manMatMan.push(matMan);
		setupCamera(matMan, camPos, camRot);

		renderSkybox(matMan, skyboxShader, skybox);

		manMatMan.setMode(matMan, MATRIX_MODE_MODEL);
		manRenderer.renderModel(villageModel, villageShader, matMan);

		manMatMan.setMode(matMan, MATRIX_MODE_MODEL);
		manGameObjRegist.render(gameObjRegist, frameDelta);

	manMatMan.setMode(matMan, MATRIX_MODE_VIEW);
	manMatMan.pop(matMan);
}

static void onRender(GameLoop* self, float frameDelta) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;
	render(frameDelta, self->primaryWindow, data->camPos, data->camRot, data->skybox, data->skyboxShader, data->villageShader, data->villageModel, data->gameObjRegist);
}
