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
#include "col/CollisionDetection.h"
#include "col/CollisionResponse.h"

GameLoop* gameloop;

static void onCreate(GameLoop* self);
static void onInitWindow(GameLoop* self);
static void onInitOpenGL(GameLoop* self);
static void onInitMisc(GameLoop* self);
static void onUpdate(GameLoop* self, float tickDelta);
static void onRender(GameLoop* self, float frameDelta);
static void onClose(GameLoop* self);
static void onDestroy(GameLoop* self);

void runPhysCollisionTest() {
	gameloop = manGameLoop.new(&onCreate, &onInitWindow, &onInitOpenGL, &onInitMisc, &onUpdate, &onRender, &onClose, &onDestroy, 1/120.0, 1/60.0);
	manGameLoop.enterGameLoop(gameloop);
	manGameLoop.delete(gameloop);
}

Shader *shader;
Shader *shader2;
Vec3 *camPos;
Vec3 *camRot;
MatrixManager *manMat;

ColliderSimpleMesh* cubeCollider;
PhysicsCollider* cube1;
PhysicsCollider* cube2;

VAO *cubeVAO;

Skybox *skybox;
Shader *skyboxShader;

Particle *particle;
Particle *particle2;
ParticleForceRegistry *particleForceRegistry;
GravityForceGenerator *gravityFG;
AnchoredSpringForceGenerator *anchoredSpringFG;
Vec3 *gravity;
Vec3 *anchor;


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

static void onCreate(GameLoop* self) {
	camPos = malloc(sizeof(Vec3));
	camRot = malloc(sizeof(Vec3));
	manMat = manMatMan.new();

	cubeCollider = makeCube();

	particle = manParticle.new();
	particle2 = manParticle.new();
	particleForceRegistry = manForceRegistry.new();
	gravity = malloc(sizeof(Vec3));
	anchor = malloc(sizeof(Vec3));
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

	shader = manShader.newFromGroup("./data/shaders/", "passThru");
	shader2 = manShader.newFromGroup("./data/shaders/", "passThru2");

	skybox = manSkybox.new(	"./data/texture/purplenebula_front.bmp", "./data/texture/purplenebula_back.bmp",
						    "./data/texture/purplenebula_top.bmp",   "./data/texture/purplenebula_top.bmp",
							"./data/texture/purplenebula_left.bmp",  "./data/texture/purplenebula_right.bmp");
	skyboxShader = manShader.newFromGroup("./data/shaders/", "skybox");

	cubeVAO = objLoader.genVAOFromFile("./data/models/cube.obj");

	glUseProgram(shader->program);
	glUniform1i(glGetUniformLocation(shader->program, "tex"), 0);

	// Create and register GravityForceGenerator
	manVec3.create(gravity, 0.0f, -9.8f, 0.0f);
	gravityFG = manGravityForceGenerator.new(gravity);
	manForceRegistry.add(particleForceRegistry, particle, &(gravityFG->forceGenerator));

	// Create and register AnchoredSpringForceGenerator
	manVec3.create(anchor, 0.0f, 1.0f, 0.0f);
	anchoredSpringFG = manAnchoredSpringForceGenerator.new(anchor, 2.00f, 0.2f, 0.0f);
	manForceRegistry.add(particleForceRegistry, particle, &(anchoredSpringFG->forceGenerator));

}

static void onInitMisc(GameLoop* self) {
	manVec3.create(camPos, 0,0,0);
	manVec3.create(camRot, 0,0,0);

	manMatMan.setMode(manMat, MATRIX_MODE_PROJECTION);
	manMatMan.pushPerspective(manMat, 1.152f, (float)manWin.getWidth(self->primaryWindow)/(float)manWin.getHeight(self->primaryWindow), 0.1, 100);
	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.pushIdentity(manMat);
	manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
	manMatMan.pushIdentity(manMat);

	cube1 = manPhysCollider.new(&particle2->position, NULL, NULL, &particle2->velocity);
	manPhysCollider.setBroadphase(cube1, NULL, 1);
	manPhysCollider.attachNarrowphaseSimpleMesh(cube1, cubeCollider);

	cube2 = manPhysCollider.new(&particle->position, NULL, NULL, &particle->velocity);
	manPhysCollider.setBroadphase(cube2, NULL, 1);
	manPhysCollider.attachNarrowphaseSimpleMesh(cube2, cubeCollider);
}

static void onUpdate(GameLoop* self, float tickDelta) {
	float rate = 20.0f;

	if (manKeyboard.isDown(self->primaryWindow, KEY_LSHIFT)) {
		rate = 0.5f;
	}

	if(manMouse.isDown(self->primaryWindow, MOUSE_BUTTON_RIGHT)) {
		camRot->x -= manMouse.getDY(self->primaryWindow)/100;
		camRot->y += manMouse.getDX(self->primaryWindow)/100;

		if(manKeyboard.isDown(self->primaryWindow, KEY_W)) {
			camPos->x +=  rate*cos(camRot->y-1.57079632679)*tickDelta;
			camPos->z +=  rate*sin(camRot->y-1.57079632679)*tickDelta;
			camPos->y += -rate*sin(camRot->x)*tickDelta;
		}

		if(manKeyboard.isDown(self->primaryWindow, KEY_S)) {
			camPos->x +=  rate*cos(camRot->y+1.57079632679)*tickDelta;
			camPos->z +=  rate*sin(camRot->y+1.57079632679)*tickDelta;
			camPos->y +=  rate*sin(camRot->x)*tickDelta;
		}

		if(manKeyboard.isDown(self->primaryWindow, KEY_A)) {
			camPos->x += -rate*cos(camRot->y)*tickDelta;
			camPos->z += -rate*sin(camRot->y)*tickDelta;
		}

		if(manKeyboard.isDown(self->primaryWindow, KEY_D)) {
			camPos->x += rate*cos(camRot->y)*tickDelta;
			camPos->z += rate*sin(camRot->y)*tickDelta;
		}
	} else {
		//Rotate Up
		if(manKeyboard.isDown(self->primaryWindow, KEY_I))
			cube1->rotation->x +=  rate;
		//Rotate left
		if(manKeyboard.isDown(self->primaryWindow, KEY_J))
			cube1->rotation->y +=  rate;
		//Rotate right
		if(manKeyboard.isDown(self->primaryWindow, KEY_L))
			cube1->rotation->y -=  rate;
		//Rotate Down
		if(manKeyboard.isDown(self->primaryWindow, KEY_K))
			cube1->rotation->x -=  rate;
		//Roll CW
		if(manKeyboard.isDown(self->primaryWindow, KEY_U))
			cube1->rotation->z +=  rate;
		//Roll CCW
		if(manKeyboard.isDown(self->primaryWindow, KEY_O))
			cube1->rotation->z -=  rate;

		rate *= 5;
		Vec3 movement = {0,0,0};
		//Forward
		if(manKeyboard.isDown(self->primaryWindow, KEY_W))
			movement.z +=  rate;
		//Left
		if(manKeyboard.isDown(self->primaryWindow, KEY_A))
			movement.x +=  rate;
		//Backwards
		if(manKeyboard.isDown(self->primaryWindow, KEY_S))
			movement.z -=  rate;
		//Right
		if(manKeyboard.isDown(self->primaryWindow, KEY_D))
			movement.x -=  rate;
		//Up
		if(manKeyboard.isDown(self->primaryWindow, KEY_Q))
			movement.y +=  rate;
		//Down
		if(manKeyboard.isDown(self->primaryWindow, KEY_E))
			movement.y -=  rate;

		manParticle.addForce(particle2, &movement);

		//Stop
		if(manKeyboard.isDown(self->primaryWindow, KEY_SPACE))
			*cube1->velocity = manVec3.create(NULL, 0,0,0);

	}

	// Update forces acting on particle
	manForceRegistry.updateForces(particleForceRegistry, tickDelta);

	// Update particle
	manParticle.integrate(particle, tickDelta);
	manParticle.integrate(particle2, tickDelta);


	if (manColDetection.checkStaticBroadphase(cube1, cube2)) {
		CollisionResult info = manColDetection.checkStaticNarrowphase(cube1, cube2);
		if (info.isColliding) {
			Vec3 translation = manVec3.preMulScalar(info.distance/2, &info.axis);

			*cube1->position = manVec3.sum(cube1->position, &translation);
			translation = manVec3.invert(&translation);
			*cube2->position = manVec3.sum(cube2->position, &translation);

			momentumCollisionResponse(cube1->velocity, cube2->velocity, *cube1->velocity, *cube2->velocity, 1/particle2->inverseMass, 1/particle->inverseMass);
		}
	}
}

static void bindMatricies(Shader* sha, MatrixManager* mats) {
	manShader.bindUniformMat4(sha, "projectionMatrix", manMatMan.peekStack(mats, MATRIX_MODE_PROJECTION));
	manShader.bindUniformMat4(sha, "viewMatrix",       manMatMan.peekStack(mats, MATRIX_MODE_VIEW));
	manShader.bindUniformMat4(sha, "modelMatrix",      manMatMan.peekStack(mats, MATRIX_MODE_MODEL));
}

static void renderSkybox() {
	manShader.bind(skyboxShader);
		bindMatricies(skyboxShader, manMat);
		manSkybox.draw(skybox, skyboxShader, "cubeTexture");
	manShader.unbind();
}

static const Vec3 xAxis = {1, 0, 0};
static const Vec3 yAxis = {0, 1, 0};
static const Vec3 zAxis = {0, 0, 1};

static void setupCamera() {
	manMatMan.rotate(manMat, camRot->x, xAxis);
	manMatMan.rotate(manMat, camRot->y, yAxis);
	manMatMan.rotate(manMat, camRot->z, zAxis);
	manMatMan.translate(manMat, *camPos);
}

static void drawVAO(VAO* vao, Shader* sha, MatrixManager* mats, Vec3* pos, Vec3* rot, Vec3* scale) {
	manMatMan.push(manMat);
		manMatMan.translate(mats, *pos);
		manMatMan.scale(mats, *scale);
		manMatMan.rotate(mats, rot->x, xAxis);
		manMatMan.rotate(mats, rot->y, yAxis);
		manMatMan.rotate(mats, rot->z, zAxis);

		bindMatricies(sha, mats);

		manVAO.draw(vao);
	manMatMan.pop(manMat);
}

static void onRender(GameLoop* self, float frameDelta) {
	glViewport(0, 0, manWin.getFramebufferWidth(self->primaryWindow), manWin.getFramebufferHeight(self->primaryWindow));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.push(manMat);
		setupCamera();

		renderSkybox();

		manShader.bind(shader);
			manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
			drawVAO(cubeVAO, shader, manMat, cube1->position, cube1->rotation, cube1->scale);
		manShader.unbind();

		manShader.bind(shader2);
			manMatMan.setMode(manMat, MATRIX_MODE_MODEL);
			drawVAO(cubeVAO, shader2, manMat, cube2->position, cube2->rotation, cube2->scale);
		manShader.unbind();

	manMatMan.setMode(manMat, MATRIX_MODE_VIEW);
	manMatMan.pop(manMat);
}

static void onClose(GameLoop* self) {
	free(shader);
	/*free(cube1->position);
	free(cube1->rotation);
	free(cube1->scale);
	free(cube1->velocity);
	free(cube2->position);
	free(cube2->rotation);
	free(cube2->scale);
	free(cube2->velocity);*/
}

static void onDestroy(GameLoop* self) {
	manMatMan.delete(manMat);
	free(manMat);
	free(camPos);
	free(camRot);
	manColMesh.deleteSimpleMesh(cubeCollider);
	free(cubeCollider);
}
