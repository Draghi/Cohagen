#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "Particle.h"

static Particle *new();
static void delete(Particle *particle);
static void integrate(Particle *const particle, scalar frameTime);
static void clearForceAccumulator(Particle *const particle);
static void addForce(Particle *const particle, const Vec3 *const force);

static Particle *new() {
	Particle *particle = calloc(1, sizeof(Particle));

	particle->damping = 0.1f;
	particle->inverseMass = 1.0f;

	return particle;
}

static void delete(Particle *particle) {
	free(particle);
}

static void integrate(Particle *const particle, scalar frameTime) {
	assert(frameTime > 0.0);

	printf("%f \n", frameTime);

	// Update position
	Vec3 positionModifier = manVec3.postMulScalar(&(particle->velocity), frameTime);
	particle->position = manVec3.sum(&(particle->position), &positionModifier);
	printf("%f %f %f\n", particle->position.x, particle->position.y, particle->position.z);

	// Determine acceleration from force
	Vec3 accelerationDueToForce = manVec3.postMulScalar(&(particle->forceAccum), particle->inverseMass);
	particle->acceleration = manVec3.sum(&(particle->acceleration), &accelerationDueToForce);

	// Update velocity from acceleration
	particle->velocity = manVec3.postMulScalar(&(particle->acceleration), frameTime);

	// Apply drag
	particle->velocity = manVec3.postMulScalar(&(particle->velocity), pow(particle->damping, frameTime));

	// Clear forces
	manParticle.clearForceAccumulator(particle);
}

static void clearForceAccumulator(Particle *const particle) {
	particle->forceAccum = manVec3.create(NULL, 0.0f, 0.0f, 0.0f);
}

static void addForce(Particle *const particle, const Vec3 *const force) {
	particle->forceAccum = manVec3.sum(&(particle->forceAccum), force);	
}

static scalar getMass(Particle *const particle) {
	return (1 / particle->inverseMass);
}

static bool hasFiniteMass(Particle *const particle) {
	bool result = false;

	if (particle->inverseMass != 0) {
		result = true;
	}

	return result;
}

const ParticleManager manParticle = {new, delete, integrate, clearForceAccumulator, addForce, getMass, hasFiniteMass};