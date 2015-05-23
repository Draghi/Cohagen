#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "Particle.h"

static Particle *new();
static void delete(Particle *particle);
static void integrate(Particle *const particle, scalar frameTime);
static void clearForceAccumulator(Particle *const particle);
static void addForce(Particle *const particle, const Vec3 *const force);
static bool hasFiniteMass(Particle *const particle);

static Particle *new() {
	Particle *particle = calloc(1, sizeof(Particle));

	particle->damping = 0.8f;
	particle->inverseMass = 1.0f;
	particle->position = manVec3.create(NULL, 0.0f, 0.0f, 0.0f);
	particle->velocity = manVec3.create(NULL, 0.0f, 0.0f, 0.0f);
	particle->acceleration = manVec3.create(NULL, 0.0f, 0.0f, 0.0f);
	particle->forceAccum = manVec3.create(NULL, 0.0f, 0.0f, 0.0f);

	return particle;
}

static void delete(Particle *particle) {
	free(particle);
}

static void integrate(Particle *const particle, scalar frameTime) {
	assert(frameTime > 0.0);


	// Update position
	Vec3 positionModifier = manVec3.postMulScalar(&(particle->velocity), frameTime);
	particle->position = manVec3.sum(&(particle->position), &positionModifier);

	// Determine acceleration from force
	Vec3 accelerationDueToForce = manVec3.postMulScalar(&(particle->forceAccum), particle->inverseMass);
	accelerationDueToForce = manVec3.sum(&(particle->acceleration), &accelerationDueToForce);

	// Update velocity from acceleration
	Vec3 velocityModifer = manVec3.postMulScalar(&accelerationDueToForce, frameTime);
	particle->velocity = manVec3.sum(&(particle->velocity), &velocityModifer);

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
	if (hasFiniteMass(particle)) {
		return (1 / particle->inverseMass);
	} else {
		return (SCALAR_MAX_VAL);
	}
}

static bool hasFiniteMass(Particle *const particle) {
	return (particle->inverseMass >= 0.0f);
}

static void setMass(Particle *const particle, scalar mass) {
	if (mass != 0) {
		particle->inverseMass = ((scalar)1.0 / mass);
	}
}

static void setInverseMass(Particle *const particle, scalar inverseMass) {
	particle->inverseMass = inverseMass;
}

static void setVelocity(Particle *const particle, const Vec3 *const velocity) {
	particle->velocity = *velocity;
}

static void setVelocityXYZ(Particle *const particle, float x, float y, float z) {
	particle->velocity.x = x;
	particle->velocity.y = y;
	particle->velocity.z = z;
}

static void setAccelerationXYZ(Particle *const particle, float x, float y, float z) {
	particle->acceleration.x = x;
	particle->acceleration.y = y;
	particle->acceleration.z = z;
}

static void setDamping(Particle *const particle, float damping) {
	particle->damping = damping;	
}

static float getDamping(Particle *const particle) {
	return (particle->damping);	
}

static void setPositionXYZ(Particle *const particle, float x, float y, float z) {
	particle->position.x = x;
	particle->position.y = y;
	particle->position.z = z;	
}

static Vec3 getPosition(const Particle *const particle, Vec3 *const vector) {
	Vec3 pos = manVec3.createFromVec3(NULL, &particle->position);

	if (vector != NULL) {
		vector->x = pos.x;
		vector->y = pos.y;
		vector->z = pos.z;
	}

	return pos;
}

const ParticleManager manParticle = {new, delete, integrate, clearForceAccumulator, addForce, getMass, hasFiniteMass, setMass, setInverseMass, setVelocity, setVelocityXYZ, setAccelerationXYZ, setDamping, getDamping, setPositionXYZ, getPosition};
