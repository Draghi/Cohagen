#ifndef BALLISTIC_TEST_H
#define BALLISTIC_TEST_H

#include "physics/Particle.h"
#include "lib/ogl.h"
#include "util/OGLUtil.h"
#include "glut/Display.h"
#include "gl/Shader.h"
#include "util/ObjLoader.h"

#include <GL/glut.h>

// Must use pre-processor directive to avoid "variably modifed array at file scope error". Cannot use constants
#define MAX_AMMO_ROUNDS 16

void ballisticTest();

typedef enum ShotType_s {
	UNUSED = 0,
	PISTOL
} ShotType;

typedef struct AmmoRound_s {
	Particle particle;
	ShotType shotType;
	unsigned int startTime;
} AmmoRound;


typedef struct BallisticSimulation_s {
	AmmoRound 	ammoRound[MAX_AMMO_ROUNDS];
	ShotType 	currentShotType;
} BallisticSimulation;

typedef struct BallisticSimulationManager_s {
	BallisticSimulation *(*new)();
	
	void (*delete)(BallisticSimulation *simToDestroy);

	void (*fireRound)(BallisticSimulation *sim);

	void (*update)(BallisticSimulation *sim);
} BallisticSimulationManager;

extern const BallisticSimulationManager manBallisticSimulation;

#endif