#ifndef ANCHORED_SPRING_FORCE_GENERATOR_H
#define ANCHORED_SPRING_FORCE_GENERATOR_H

#include "ParticleForceGenerator.h"

/**
 *	Spring is attached to a single, fixed point rather than to another particle.
 */
typedef struct AnchoredSpringForceGenerator_s {
	/**
	 *	Particle force generator structure.
	 */
	ParticleForceGenerator forceGenerator;

	/**
	 *	Point to anchor spring to.
	 */
	Vec3 *anchor;

	/**
	 *	Spring constant.
	 */
	scalar springConstant;

	/**
	 *	Spring dampening.
	 */
	scalar springDamp;

	/**
	 *	Rest length of spring.
	 */
	scalar restLength;
} AnchoredSpringForceGenerator;

typedef struct AnchoredSpringForceGeneratorManager_s {
	/**
	 *	Create a new AnchoredSpringForceGenerator with the given parameters.
	 *
	 * 	@param 	anchor 			const pointer to Vec3, anchor point of spring.
	 * 	@param 	springConstant 	scalar, spring constant for spring.
	 * 	@param 	springConstant 	scalar, dampening for the spring.
	 * 	@param 	restLength 		scalar, the resting length of the spring.
	 * 	@return 				pointer to AnchoredSpringForceGenerator, the constructed AnchoredSpringForceGenerator.
	 */
	AnchoredSpringForceGenerator *(*new)(Vec3 *const anchor, scalar springConstant, scalar springDamp, scalar restLength);

	/**
	 *	Free all memory associated with the given AnchoredSpringForceGenerator.
	 */
	void (*delete)(AnchoredSpringForceGenerator *forceGenerator);
} AnchoredSpringForceGeneratorManager;

extern const AnchoredSpringForceGeneratorManager manAnchoredSpringForceGenerator;

#endif
