#include <stdio.h>

#include "GravityForceGenerator.h"

static void updateForce(void *const self, Particle *const particle, scalar frameTime);

static GravityForceGenerator *new(const Vec3 *const gravityAccel) {
	GravityForceGenerator *fg = malloc(sizeof(GravityForceGenerator));

	fg->gravity = manVec3.createFromVec3(NULL, gravityAccel);

	ParticleForceGenerator *tempForceGen = manParticleForceGenerator.new(updateForce);

	fg->forceGenerator = *tempForceGen;

	manParticleForceGenerator.delete(tempForceGen);

	return fg;
}

static void delete(GravityForceGenerator *gravityForceGenerator) {
	free(gravityForceGenerator);
}

static void updateForce(void *const self, Particle *const particle, scalar frameTime) {
	GravityForceGenerator *fg = (GravityForceGenerator *) self;

	Vec3 forceToAdd = manVec3.postMulScalar(&(fg->gravity), manParticle.getMass(particle));

	manParticle.addForce(particle, &forceToAdd);
}

const GravityForceGeneratorManager manGravityForceGenerator = {new, delete};