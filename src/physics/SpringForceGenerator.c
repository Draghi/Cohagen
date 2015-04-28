#include "SpringForceGenerator.h"

static void updateForce(void *const self, Particle *const particle, scalar frameTime);

static SpringForceGenerator *new(Particle *const otherParticle, scalar springConstant, scalar restLength) {
	SpringForceGenerator *fg = malloc(sizeof(SpringForceGenerator));

	fg->other = otherParticle;
	fg->springConstant = springConstant;
	fg->restLength = restLength;

	ParticleForceGenerator *tempForceGen = manParticleForceGenerator.new(updateForce);
	
	fg->forceGenerator = *tempForceGen;

	manParticleForceGenerator.delete(tempForceGen);

	return fg;
}

static void delete(SpringForceGenerator *forceGenerator) {
	free(forceGenerator);
}

static void updateForce(void *const self, Particle *const particle, scalar frameTime) {
	SpringForceGenerator *fg = (SpringForceGenerator *) self;

	// Force of the spring | f = -k(magnitude(vectorOfSpring) - restLength)*normalize(vectorOfSpring)
	Vec3 force = manVec3.createFromVec3(NULL, &(particle->position));
	force = manVec3.sub(&force, &(fg->other->position));

	scalar magnitude = manVec3.magnitude(&force);
	magnitude = abs(magnitude - fg->restLength);
	magnitude *= fg->springConstant;

	force = manVec3.normalize(&force);
	force = manVec3.postMulScalar(&force, -magnitude);

	manParticle.addForce(particle, &force);
}

const SpringForceGeneratorManager manSpringForceGenerator = {new, delete};