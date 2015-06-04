#ifndef BALLISTICS_H
#define BALLISTICS_H

#include "physics/Particle.h"
#include "gl/Shader.h"
#include "gl/VAO.h"

#define MAX_NUM_ROUNDS 16

typedef enum ballisticType_e {
	UNUSED = 0,
	PISTOL,
	ARTILLERY,
	LASER
} BallisticType;

typedef struct BallisticRepresentation_s {
	Shader *shader;
	VAO *vao;
} BallisticRepresentation;

typedef struct BallisticRound_s {
	Particle 					*particle;
	BallisticType 				type;
	unsigned int 				startTime;
	BallisticRepresentation 	*representation;

	void (*render)(const struct BallisticRound_s *const round, const BallisticRepresentation *const representation);
} BallisticRound;

typedef struct BallisticSim_s {
	BallisticRound 	rounds[MAX_NUM_ROUNDS];
	BallisticType 	currentShotType;
} BallisticSim;

typedef struct BallisticManager_s {
	BallisticSim *(*new)(const Vec3 *const launchPoint);
	void (*delete)(BallisticSim *ballisticSim);

	void (*renderAll)(BallisticSim *const ballisticSim);
	void (*update)(BallisticSim *const ballisticSim);
	void (*fire)(BallisticSim *const ballisticSim);
} BallisticManager;

extern const BallisticManager manBallistic;

#endif // BALLISTICS_H
