#ifndef GRAVITY_FORCE_GENERATOR_H
#define GRAVITY_FORCE_GENERATOR_H

#include "ParticleForceGenerator.h"

/**
 *	Force generator that applies force due to
 * 	gravity each frame. Most likely won't use
 * 	this in practice. Instead apply gravity directly
 * 	as an acceleration.
 */
typedef struct GravityForceGenerator_s {
	/**
	 *	Force Generator instance to use.
	 */
	ParticleForceGenerator forceGenerator;

	/**
	 *	Acceleration due to gravity to apply each step.
	 */
	Vec3 gravity;
} GravityForceGenerator;

/** 
 *	Manager for GravityForce Generators.
 * 	As per usual, all interaction with GravityForceGenerators
 * 	should be via the manager "singleton".
 */
typedef struct GravityForceGeneratorManager_s {
	/**
	 *	Instantiate a new GravityForceGenerator 
	 *
	 * 	@param 	gravityAccel 	const pointer to const Vec3, accleration due to gravity vector (world space).
	 * 	@return 				pointer to GravityForceGenerator, new GravityForceGenerator.
	 */
	GravityForceGenerator *(*new)(const Vec3 *const gravityAccel);

	/** 
	 *	Free all memory associated with a given GravityForceGenerator.
	 * 	
	 *	@param 	gravityForceGenerator 	pointer to GravityForceGenerator to delete.
	 */
	void (*delete)(GravityForceGenerator *gravityForceGenerator);
} GravityForceGeneratorManager;

extern const GravityForceGeneratorManager manGravityForceGenerator;

#endif