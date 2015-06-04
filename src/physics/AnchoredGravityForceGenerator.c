#include <stdio.h>

#include "AnchoredGravityForceGenerator.h"

static const scalar GRAVITATIONAL_CONSTANT = 6.67384e-11; //6.67384 * 10^-11

static void updateForce(void *const self, Particle *const particle, scalar frameTime);

static AnchoredGravityForceGenerator* new(Particle *gravityAnchor) {
	AnchoredGravityForceGenerator *fg = malloc(sizeof(AnchoredGravityForceGenerator));

	fg->gravityAnchor = gravityAnchor;

	ParticleForceGenerator *tempForceGen = manParticleForceGenerator.new(fg, updateForce);
	
	fg->forceGenerator = *tempForceGen;
	
	manParticleForceGenerator.delete(tempForceGen);

	return fg;
}

static void delete(AnchoredGravityForceGenerator *anchoredGravityForceGenerator) {
	//Do nothing
}

static void updateForce(void *const self, Particle *const particle, scalar frameTime) {
	ParticleForceGenerator *pfg = (ParticleForceGenerator *) self;
	AnchoredGravityForceGenerator *fg = (AnchoredGravityForceGenerator *) pfg->self;

	Vec3 diff = manVec3.sub(particle->position, fg->gravityAnchor->position);
	scalar mag = manVec3.magnitude(&diff);
	if (mag != 0) {
		Vec3 dir = manVec3.create(NULL, diff.x/mag, diff.y/mag, diff.z/mag);

		scalar grav = GRAVITATIONAL_CONSTANT*((manParticle.getMass(fg->gravityAnchor)*manParticle.getMass(particle))/(mag*mag));
		Vec3 force = manVec3.preMulScalar(-grav, &dir);

		manParticle.addForce(particle, &force);
	}
}

const AnchoredGravityForceGeneratorManager manAnchoredGravityForceGenerator = {new, delete};
