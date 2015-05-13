#include <stdio.h>
#include <math.h>

#include "AnchoredSpringForceGenerator.h"

static void updateForce(void *const self, Particle *const particle, scalar frameTime);

static AnchoredSpringForceGenerator *new(Vec3 *const anchor, scalar springConstant, scalar restLength) {
	AnchoredSpringForceGenerator *fg = malloc(sizeof(AnchoredSpringForceGenerator));

	fg->anchor = anchor;
	fg->springConstant = springConstant;
	fg->restLength = restLength;

	ParticleForceGenerator *tempForceGen = manParticleForceGenerator.new(fg, updateForce);

	fg->forceGenerator = *tempForceGen;

	manParticleForceGenerator.delete(tempForceGen);

	return fg;
}

static void delete(AnchoredSpringForceGenerator *forceGenerator) {
	free(forceGenerator);
}

static void updateForce(void *const self, Particle *const particle, scalar frameTime) {
	ParticleForceGenerator *pfg = (ParticleForceGenerator *) self;
	AnchoredSpringForceGenerator *fg = (AnchoredSpringForceGenerator *) pfg->self;

	// Calculate the vector of the spring
	Vec3 force = manParticle.getPosition(particle, NULL);
	printf("Force: %f %f %f\n", force.x, force.y, force.z);
	force = manVec3.sub(&force, fg->anchor);

	// Calculate the magnitude of the force
	scalar magnitude = manVec3.magnitude(&force);
	magnitude = fabs(magnitude - fg->restLength);
	magnitude *= fg->springConstant;

	// Calculate and apply the final force
	force = manVec3.normalize(&force);
	force = manVec3.postMulScalar(&force, -magnitude);

	manParticle.addForce(particle, &force);
}

const AnchoredSpringForceGeneratorManager manAnchoredSpringForceGenerator = {new, delete};
