#include <stdio.h>
#include <math.h>

#include "AnchoredSpringForceGenerator.h"

static void updateForce(void *const self, Particle *const particle, scalar frameTime);

static AnchoredSpringForceGenerator *new(Vec3 *const anchor, scalar springConstant, scalar springDamp, scalar restLength) {
	AnchoredSpringForceGenerator *fg = malloc(sizeof(AnchoredSpringForceGenerator));

	fg->anchor = anchor;
	fg->springConstant = springConstant;
	fg->springDamp = springDamp;
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

	// Vec3 pPos = manParticle.getPosition(particle, NULL);
	// Vec3 pVel = particle->velocity;

	// Vec3 diff = manVec3.sub(&pPos, fg->anchor);
	// scalar distance = manVec3.magnitude(&diff);

	// Vec3 direction;
	// if (distance == 0) {
	// 	//Attempt to find the best direction to push the spring, if we're ontop of the anchor.
	// 	if (manVec3.magnitude(&pVel) != 0) {
	// 		direction = manVec3.normalize(&pVel);

	// 	} else if (manVec3.magnitude(&particle->acceleration) != 0) {
	// 		direction = manVec3.normalize(&particle->acceleration);

	// 	} else if (manVec3.magnitude(&particle->forceAccum) != 0) {
	// 		direction = manVec3.normalize(&particle->forceAccum);

	// 	} else {
	// 		//Arbitrary fallback direction.
	// 		direction = manVec3.create(NULL, 0, 1, 0);
	// 	}
	// } else
	// 	direction = manVec3.normalize(&diff);

	// Vec3 damp = manVec3.preMulScalar(fg->springDamp, &pVel);

	// Vec3 force = manVec3.preMulScalar(-fg->springConstant*(distance-fg->restLength), &direction);
	// force = manVec3.sub(&force, &damp);

	// manParticle.addForce(particle, &force);

	// Find the vector from the particle's current position to the anchor's current position
	// Calculate the direction of the force to apply
	Vec3 force = manParticle.getPosition(particle, NULL);
	force = manVec3.sub(&force, fg->anchor);

	// Calculate the magnitude of the force to apply
	scalar magnitude = manVec3.magnitude(&force);
	magnitude = scalar_abs(magnitude - fg->restLength);
	magnitude *= fg->springConstant;

	// force = manVec3.normalize(&force);

	// Spring damping
	Vec3 damp = manVec3.preMulScalar(fg->springDamp, &force);

	// Required force
	force = manVec3.postMulScalar(&force, -magnitude);

	// Apply damping
	force = manVec3.sub(&force, &damp);

	manParticle.addForce(particle, &force);
}

const AnchoredSpringForceGeneratorManager manAnchoredSpringForceGenerator = {new, delete};
