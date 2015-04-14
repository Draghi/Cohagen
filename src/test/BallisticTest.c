#include <stdio.h>
#include <math.h>

#include "BallisticTest.h"

static void setupOpenGL();
static void setupDisplay();
static void setupBallisticSim();
static void ballisticDisplay();
static void ballisticUpdate();
static Mat4 createProjectionMatrix(float verticalFovRad, float nearZClip, float farZClip, float aspectRatio);
static void keyboard(unsigned char key, int x, int y);

static Mat4 worldMatrix;
static Mat4 viewMatrix;
static Mat4 projMatrix;
static int prevTime = 0;
static int deltaTime = 0.0f;
static BallisticSimulation *sim;
static GLuint vao;
static int numIndicesToDraw;
static Shader *simShader;

void ballisticTest() {
	setupDisplay();
	setupOpenGL();
	setupBallisticSim();
}

static void ballisticDisplay() {
	float elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = elapsedTime - prevTime;
	deltaTime = deltaTime == 0 ? 1 : deltaTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 	for (int i = 0; i < MAX_AMMO_ROUNDS; ++i) {
 		if (sim->ammoRound[i].shotType != UNUSED) {
			worldMatrix.data[3].x = sim->ammoRound[i].particle.position.x;
			worldMatrix.data[3].y = sim->ammoRound[i].particle.position.y;
			worldMatrix.data[3].z = sim->ammoRound[i].particle.position.z;

			manShader.bindUniformMat4(simShader, "worldMatrix", &worldMatrix);
			manShader.bindUniformMat4(simShader, "viewMatrix", &viewMatrix);
			manShader.bindUniformMat4(simShader, "projMatrix", &projMatrix);

			manShader.bind(simShader);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			manShader.unbind();
 		}
 	}


	prevTime = elapsedTime;
}

static void ballisticUpdate() {
	manBallisticSimulation.update(sim);

	// printf("%d\n", deltaTime);
	// printf("%d\n", prevTime);
	if (prevTime >= 3000.0f && sim->ammoRound[0].shotType != PISTOL) {
		printf("FIRED TIME\n");
		manBallisticSimulation.fireRound(sim);
	}
}

static void setupDisplay() {

	display.setWindowSize(800, 600); //800x600 window
	display.doCenterWindow(); //Make sure the window pops up in the center

	display.setDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE); //Double buffered, RGB + depth buffer, Multisampled window.
	display.setOGLVersion(3, 3); //Set the context to OGL 3.3

	display.setVirtualFPS(60); //Sets the virtual FPS
	display.setVirtualTPS(60); //Sets the virtual TPS

	display.setUpdateCallback(ballisticUpdate);
	display.setRenderCallback(ballisticDisplay);

	glutKeyboardFunc(keyboard);

	display.createWindow(); //Create the window
}

static void setupOpenGL() {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	manOGLUtil.setBackfaceCulling(GL_CCW);
}

static void setupBallisticSim() {
	sim = manBallisticSimulation.new();

	vao = objLoader.genVAOFromFile("./build/data/models/dragon_smooth.obj", &numIndicesToDraw);
	simShader = manShader.newFromGroup("./build/data/shaders/", "ballistic");

	worldMatrix = manMat4.createLeading(NULL, 1.0f);
	viewMatrix = manMat4.createLeading(NULL, 1.0f);
	viewMatrix.data[3].z = -20.0f;
	projMatrix = createProjectionMatrix(1.152f, 0.1f, 100.0f, display.getWindowWidth()/(float)display.getWindowHeight());

	manShader.bindUniformMat4(simShader, "worldMatrix", &worldMatrix);
	manShader.bindUniformMat4(simShader, "viewMatrix", &viewMatrix);
	manShader.bindUniformMat4(simShader, "projMatrix", &projMatrix);
}

// static void renderAmmoRound() {

// }

static BallisticSimulation *new() {
	BallisticSimulation *sim = malloc(sizeof(BallisticSimulation));

	sim->currentShotType = PISTOL;

	// Loop thru all shots and set them to UNUSED
	for (int i = 0; i < MAX_AMMO_ROUNDS; ++i) {
		sim->ammoRound[i].shotType = UNUSED;
	}

	return sim;
}

static void delete(BallisticSimulation *simToDestroy) {
	free(simToDestroy);
}

static void fireRound(BallisticSimulation *sim) {
	int shotIndex = 0;
	// Look for first UNUSED slot
	for (; shotIndex < MAX_AMMO_ROUNDS; ++shotIndex) {
		if (sim->ammoRound[shotIndex].shotType == UNUSED) {
			break;
		}
	}

	// Only continue if we found an unused shot.
	if (shotIndex >= 0 && shotIndex < MAX_AMMO_ROUNDS) {
		AmmoRound *shot = &(sim->ammoRound[shotIndex]);
			
		switch (sim->currentShotType) {
			case PISTOL:
				manParticle.setMass(&(shot->particle), 200.0f);
				manParticle.setVelocityXYZ(&(shot->particle), 4.0f, 3.0f, 0.0f);
				manParticle.setAccelerationXYZ(&(shot->particle), 0.0f, -2.0f, 0.0f);
				manParticle.setDamping(&(shot->particle), 0.99f);
				break;
			case UNUSED:
				break;
		}

		manParticle.setPositionXYZ(&(shot->particle), 0.0f, -1.5f, 0.0f);
		shot->shotType = sim->currentShotType;
		shot->startTime = glutGet(GLUT_ELAPSED_TIME);

		manParticle.clearForceAccumulator(&(shot->particle));
	}	
}

static void updateBallisticSim(BallisticSimulation *sim) {
	for (int i = 0; i < MAX_AMMO_ROUNDS; ++i) {
		if (sim->ammoRound[i].shotType != UNUSED) {
			manParticle.integrate(&(sim->ammoRound[i].particle), deltaTime/1000.0f);
		}
	}
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

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'x':

			break;
		default:
			printf("FIRED\n");
			manBallisticSimulation.fireRound(sim);
			break;
	}
}

const BallisticSimulationManager manBallisticSimulation = {new, delete, fireRound, updateBallisticSim};