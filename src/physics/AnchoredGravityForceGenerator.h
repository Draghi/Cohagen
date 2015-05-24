#ifndef COH_ANCHOREDGRAVITYFORCEGENERATOR_H
#define COH_ANCHOREDGRAVITYFORCEGENERATOR_H

#include "ParticleForceGenerator.h"

/**
 *	Force generator that applies force due to
 * 	gravity each frame. Most likely won't use
 * 	this in practice. Instead apply gravity directly
 * 	as an acceleration.
 *
 * 	Every force generator needs to have a ParticleForceGenerator as
 * 	the first member of it's structure. I use this to achieve
 * 	a sort of polymorphism.
 */
typedef struct AnchoredGravityForceGenerator_s {
	/**
	 *	Force Generator instance to use.
	 */
	ParticleForceGenerator forceGenerator;

	Particle* gravityAnchor;
	scalar gravityStrength;
} AnchoredGravityForceGenerator;

/** 
 *	Manager for GravityForce Generators.
 * 	As per usual, all interaction with GravityForceGenerators
 * 	should be via the manager "singleton".
 */
typedef struct AnchoredGravityForceGeneratorManager_s {
	/**
	 *	Instantiate a new AnchoredGravityForceGenerator
	 *
	 * 	@param 	gravityAnchor 	const pointer to const Vec3, position to pull toward.
	 * 	@param  gravityStrength The strength of the gravity well.
	 * 	@return 				pointer to AnchoredGravityForceGenerator, new AnchoredGravityForceGenerator.
	 */
	AnchoredGravityForceGenerator *(*new)(Particle *gravityAnchor);

	/** 
	 *	Free all memory associated with a given GravityForceGenerator.
	 * 	
	 *	@param 	anchoredGravityForceGenerator 	pointer to AnchoredGravityForceGenerator to delete.
	 */
	void (*delete)(AnchoredGravityForceGenerator *anchoredGravityForceGenerator);
} AnchoredGravityForceGeneratorManager;

extern const AnchoredGravityForceGeneratorManager manAnchoredGravityForceGenerator;

#endif
