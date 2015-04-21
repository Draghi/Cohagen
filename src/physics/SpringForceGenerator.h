#ifndef SPRING_FORCE_GENERATOR_H
#define SPRING_FORCE_GENERATOR_H

#include "ParticleForceGenerator.h"

/**
 * Force generator to use for spring-like forces.
 */
typedef struct SpringForceGenerator_s {
	/**
	 *	Particle force generator instance to use.
	 */
	ParticleForceGenerator *forceGenerator;

	/**
	 *	Particle at other end of spring.
	 */
	Particle *other;

	/**
	 *	Spring constant (k) value of spring.
	 */
	scalar springConstant;

	/**
	 *	Rest length of the spring.
	 */
	scalar restLength;
} SpringForceGenerator;

typedef struct SpringForceGeneratorManager_s {
	/**
	 *	Create a new spring force generator with the given parameters.
	 *
	 * 	@param 	otherParticle 	const pointer to Particle, particle on other end of spring.
	 * 	@param 	springConstant 	scalar, spring constant of the spring.
	 * 	@param 	restLength 		scalar, the resting length of the spring.
	 */
	SpringForceGenerator *(*new)(Particle *const otherParticle, scalar springConstant, scalar restLength);

	/**
	 *	Free all memory associated with a Spring Force Generator.
	 * 
	 * 	@param 	forceGenerator 	pointer to SpringForceGenerator to delete.
	 */
	void (*delete)(SpringForceGenerator *forceGenerator);
} SpringForceGeneratorManager;

extern const SpringForceGeneratorManager manSpringForceGenerator;

#endif