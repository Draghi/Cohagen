#include "Tests.h"

#include "engine/GameLoop.h"
#include "engine/Skybox.h"
#include "engine/MatrixManager.h"
#include "col/CollisionDetection.h"
#include "col/CollisionResponse.h"
#include "physics/GravityForceGenerator.h"
#include "physics/AnchoredSpringForceGenerator.h"
#include "physics/ParticleForceRegistry.h"
#include "gl/ShaderLoader.h"
#include "gl/VAO.h"
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

void runNewtonsCradle() {
	gameloop = manGameLoop.new(&onCreate, &onInitWindow, &onInitOpenGL, &onInitMisc, &onUpdate, &onRender, &onClose, &onDestroy, 1/120.0, 1/60.0);
	manGameLoop.enterGameLoop(gameloop);
	manGameLoop.delete(gameloop);
}

static ColliderSimpleMesh* makeCube() {
	const int vCount = 8;
	const int nCount = 3;

	Vec3 *verts = malloc(sizeof(Vec3)*vCount);
	verts[0] = manVec3.create(NULL, -0.5, -0.5, -0.5);
	verts[1] = manVec3.create(NULL,  0.5, -0.5, -0.5);
	verts[2] = manVec3.create(NULL, -0.5,  0.5, -0.5);
	verts[3] = manVec3.create(NULL,  0.5,  0.5, -0.5);
	verts[4] = manVec3.create(NULL, -0.5, -0.5,  0.5);
	verts[5] = manVec3.create(NULL,  0.5, -0.5,  0.5);
	verts[6] = manVec3.create(NULL, -0.5,  0.5,  0.5);
	verts[7] = manVec3.create(NULL,  0.5,  0.5,  0.5);

	Vec3 *norms = malloc(sizeof(Vec3)*nCount);
	norms[0] = manVec3.create(NULL,  0,  0,  1);
	norms[1] = manVec3.create(NULL,  0,  1,  0);
	norms[2] = manVec3.create(NULL,  1,  0,  0);


	return manColMesh.newSimpleMesh(vCount, verts, nCount, norms);
}
typedef struct NewtonsCradleData_s {
	//Particle Related
	int particleCount;
	Particle** particles;
	ParticleForceRegistry* pfRegist;
	GravityForceGenerator* gfGen;
	Vec3* gravityVec;
	AnchoredSpringForceGenerator* sfGen;
	Vec3* springAnchor;

	//Collision Related
	ColliderSimpleMesh* cubeCollider;
	PhysicsInfo** cubes;

	//Rendering related
	MatrixManager *manMat;
	Vec3 *camPos;
	Vec3 *camRot;

	//Cube Rendering
	Shader *cubeShader;
	VAO *cubeVAO;

	//Skybox Rendering
	Skybox *skybox;
	Shader *skyboxShader;

	//Village Rendering
	VAO *villageVAO;
	Shader *villageShader;
	Texture *villageTexture;

} NewtonsCradleData;

static void onCreate(GameLoop* self) {
	self->extraData = malloc(sizeof(NewtonsCradleData));
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	data->particleCount = 5;
	data->particles = malloc(sizeof(Particle*)*data->particleCount);
	data->cubes = malloc(sizeof(PhysicsInfo*)*data->particleCount);

	data->gravityVec = malloc(sizeof(Vec3));

	data->springAnchor = malloc(sizeof(Vec3));

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

	data->cubeVAO = objLoader.genVAOFromFile("./data/models/cube.obj");
	data->cubeShader = manShader.newFromGroup("./data/shaders/", "passThru");

	/** @todo: Add method to shader manager **/
	glUseProgram(data->cubeShader->program);
	glUniform1i(glGetUniformLocation(data->cubeShader->program, "tex"), 0);

	data->skybox = manSkybox.new("./data/texture/purplenebula_front.bmp", "./data/texture/purplenebula_back.bmp",
						         "./data/texture/purplenebula_top.bmp",   "./data/texture/purplenebula_top.bmp",
							     "./data/texture/purplenebula_left.bmp",  "./data/texture/purplenebula_right.bmp");
	data->skyboxShader = manShader.newFromGroup("./data/shaders/", "skybox");


	data->villageVAO     = objLoader.genVAOFromFile("./data/models/town.obj");
	data->villageShader  = manShader.newFromGroup("./data/shaders/", "house");
	data->villageTexture = textureUtil.createTextureFromFile("./data/texture/town.bmp", GL_LINEAR, GL_LINEAR);

}

static void onInitMisc(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	manVec3.create(data->gravityVec, 0, -9.8, 0);
	manVec3.create(data->springAnchor, 0, 10, 0);

	data->cubeCollider = makeCube();

	data->pfRegist = manForceRegistry.new();
	data->sfGen = manAnchoredSpringForceGenerator.new(data->springAnchor, 2.0f, 0, 0.0f);
	data->gfGen = manGravityForceGenerator.new(data->gravityVec);

	for(int i = 0; i<data->particleCount; i++) {
		data->particles[i] = manParticle.new();
		manParticle.setPositionXYZ(data->particles[i], i*1.25, 0.6, 0);

		manForceRegistry.add(data->pfRegist, data->particles[i], &data->sfGen->forceGenerator);
		manForceRegistry.add(data->pfRegist, data->particles[i], &data->gfGen->forceGenerator);

		data->cubes[i] = manPhysObj.new(&data->particles[i]->position, NULL, NULL, &data->particles[i]->velocity);
		manPhysObj.setBroadphase(data->cubes[i], NULL, 1);
		manPhysObj.attachNarrowphaseSimpleMesh(data->cubes[i], data->cubeCollider);
	}

	manVec3.create(data->camPos, 0,0,0);
	manVec3.create(data->camRot, 0,0,0);

	data->manMat = manMatMan.new();
	manMatMan.setMode(data->manMat, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(data->manMat, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.1, 100);
	manMatMan.setMode(data->manMat, MATRIX_MODE_VIEW);
	manMatMan.pushIdentity(data->manMat);
	manMatMan.setMode(data->manMat, MATRIX_MODE_MODEL);
	manMatMan.pushIdentity(data->manMat);
}

static void onClose(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	free(data->cubeVAO);
	free(data->cubeShader);

	free(data->villageVAO);
	free(data->villageShader);
	free(data->villageTexture);

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

	manAnchoredSpringForceGenerator.delete(data->sfGen);
	manGravityForceGenerator.delete(data->gfGen);
	manForceRegistry.delete(data->pfRegist);

	manColMesh.deleteSimpleMesh(data->cubeCollider);
	free(data->cubeCollider);
}

static void onDestroy(GameLoop* self) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;

	free(data->camPos);
	free(data->camRot);
	free(data->gravityVec);
	free(data->springAnchor);
	free(data->particles);
	free(data->cubes);
	free(data);
}

////////////////
// Game-logic //
////////////////

static void update(float tickDelta, Window* window, Vec3* camPos, Vec3* camRot, ParticleForceRegistry* pfRegist, int particleCount, PhysicsInfo** cubes, Particle** particles) {
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
		Vec3 force = {rate*20, 0, 0};
		manParticle.addForce(particles[0], &force);
	}

	if(manKeyboard.isDown(window, KEY_L)) {
		Vec3 force = {-rate*20, 0, 0};
		manParticle.addForce(particles[0], &force);
	}

	// Update forces acting on particle
	manForceRegistry.updateForces(pfRegist, tickDelta);

	// Update particle
	for(int i = 0; i < particleCount; i++)
		manParticle.integrate(particles[i], tickDelta);

	int i = 0;
	bool flag = true;
	while(flag) {
		flag = false;
		for(int i = 0; i < particleCount; i++) {
			for(int j = i+1; j < particleCount; j++) {
				if (manColDetection.checkStaticBroadphase(cubes[i], cubes[j])) {
					CollisionResult info = manColDetection.checkStaticNarrowphase(cubes[i], cubes[j]);
					if (info.isColliding) {
						flag = true;
						Vec3 translation = manVec3.preMulScalar(info.distance/2, &info.axis);

						*cubes[i]->position = manVec3.sum(cubes[i]->position, &translation);
						translation = manVec3.invert(&translation);
						*cubes[j]->position = manVec3.sum(cubes[j]->position, &translation);

						momentumCollisionResponse(cubes[i]->velocity, cubes[j]->velocity, *cubes[i]->velocity, *cubes[j]->velocity, 1/particles[i]->inverseMass, 1/particles[j]->inverseMass);
					}
				}
			}
		}

		if (i>9)
			flag = false;

		i++;
	}
}

static void onUpdate(GameLoop* self, float tickDelta) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;
	update(tickDelta, self->primaryWindow, data->camPos, data->camRot, data->pfRegist, data->particleCount, data->cubes, data->particles);
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

static void drawVAO(VAO* vao, Shader* sha, MatrixManager* mats, Vec3* pos, Vec3* rot, Vec3* scale) {
	manMatMan.push(mats);
		if (pos!=NULL)
			manMatMan.translate(mats, *pos);

		if (scale!=NULL)
		manMatMan.scale(mats, *scale);

		if (rot!=NULL) {
			manMatMan.rotate(mats, rot->x, xAxis);
			manMatMan.rotate(mats, rot->y, yAxis);
			manMatMan.rotate(mats, rot->z, zAxis);
		}

		bindMatricies(sha, mats);

		manVAO.draw(vao);
	manMatMan.pop(mats);
}

static void render(float frameDelta, Window* window, MatrixManager* manMat, Vec3* camPos, Vec3* camRot, Skybox* skybox, Shader* skyboxShader, VAO* villageVAO, Shader* villageShader, Texture* villageTexture, VAO* cubeVAO, Shader* cubeShader, int particleCount, PhysicsInfo** cubes) {
	glViewport(0, 0, manWin.getFramebufferWidth(window), manWin.getFramebufferHeight(window));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.push(manMat);
		setupCamera(manMat, camPos, camRot);

		renderSkybox(manMat, skyboxShader, skybox);

		manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
		manShader.bind(villageShader);
			manTex.bind(villageTexture, GL_TEXTURE_2D, 0);
				drawVAO(villageVAO, villageShader, manMat, NULL, NULL, NULL);
			manTex.unbind(villageTexture, 0);
		manShader.unbind();

		manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
		manShader.bind(cubeShader);
			for(int i = 0; i <particleCount; i++)
				drawVAO(cubeVAO, cubeShader, manMat, cubes[i]->position, cubes[i]->rotation, cubes[i]->scale);
		manShader.unbind();

	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.pop(manMat);
}

static void onRender(GameLoop* self, float frameDelta) {
	NewtonsCradleData* data = (NewtonsCradleData*)self->extraData;
	render(frameDelta, self->primaryWindow, data->manMat, data->camPos, data->camRot, data->skybox, data->skyboxShader, data->villageVAO, data->villageShader, data->villageTexture, data->cubeVAO, data->cubeShader, data->particleCount, data->cubes);
}
