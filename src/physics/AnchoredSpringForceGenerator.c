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

static scalar calcPushForce(scalar force, scalar momentum) {
	scalar res = 0;

	if (force < 0) {
		if (force<momentum) {
			res = force;
		}
	} else {
		if (force>momentum) {
			res = force;
		}
	}

	return res;
}

static void updateForce(void *const self, Particle *const particle, scalar frameTime) {
	ParticleForceGenerator *pfg = (ParticleForceGenerator *) self;
	AnchoredSpringForceGenerator *fg = (AnchoredSpringForceGenerator *) pfg->self;

	Vec3 pPos = manParticle.getPosition(particle, NULL);
	Vec3 pVel = particle->velocity;

	Vec3 diff = manVec3.sub(&pPos, fg->anchor);
	scalar distance = manVec3.magnitude(&diff);

	Vec3 direction;
	if (distance == 0) {
		//Attempt to find the best direction to push the spring, if we're ontop of the anchor.
		if (manVec3.magnitude(&pVel) != 0) {
			direction = manVec3.normalize(&pVel);

		} else if (manVec3.magnitude(&particle->acceleration) != 0) {
			direction = manVec3.normalize(&particle->acceleration);

		} else if (manVec3.magnitude(&particle->forceAccum) != 0) {
			direction = manVec3.normalize(&particle->forceAccum);

		} else {
			//Arbitrary fallback direction.
			direction = manVec3.create(NULL, 0, 1, 0);
		}
	} else
		direction = manVec3.normalize(&diff);

	Vec3 damp = manVec3.preMulScalar(fg->springDamp, &pVel);

	Vec3 force = manVec3.preMulScalar(-fg->springConstant*(distance-fg->restLength), &direction);
	force = manVec3.sub(&force, &damp);

	Vec3 pMom = manVec3.preMulScalar(manParticle.getMass(particle), &pVel);
	force.x = calcPushForce(force.x, pMom.x);
	force.y = calcPushForce(force.y, pMom.y);
	force.z = calcPushForce(force.z, pMom.z);

	manParticle.addForce(particle, &force);
}

const AnchoredSpringForceGeneratorManager manAnchoredSpringForceGenerator = {new, delete};
