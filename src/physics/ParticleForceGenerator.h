#ifndef PARTICLE_FORCE_GENERATOR_H
#define PARTICLE_FORCE_GENERATOR_H

#include "Particle.h"

/**
 *	Particle Force Generator.
 *
 * 	All Particle Force Generators must 'inherit' this class. Provides
 * 	a function which is used to apply a force to a particle every tick.
 */
typedef struct ParticleForceGenerator_s {
	/**
	 *	Pointer to the start of the structure using this force generator.
	 */
	void *self;

	/**
	 *	Pointer to function to use to update the force of the particle.
	 * 	
	 * 	@param 	self 		const pointer to void, pointer to ParticleForceGenerator. A void pointer
	 * 						was used to allow for polymorphism.
	 * 	@param 	particle 	const pointer to Particle, particle to update force of.
	 * 	@param 	frameTime 	scalar, duration of previous frame.
	 */
	void (*updateForce)(void *const self, Particle *const particle, scalar frameTime);
} ParticleForceGenerator;

/**
 *	Class used to manage Particle Force Generators.
 */
typedef struct ParticleForceGeneratorManager_s {
	/** 
	 *	Allocate memory for a particle forceGenerator using
	 * 	the given function to update the force acting on a particle each frame.
	 *
	 * 	@param 	updateForce 	pointer to function, the function to use to update the force acting on
	 * 							a particle each frame.
	 * 	@returns 				pointer to ParticleForceGenerator, the new ParticleForceGenerator.
	 */
	ParticleForceGenerator *(*new)(void *const self, void (*updateForce)(void *const self, Particle *const particle, scalar frameTime));

	/**
	 *	Free all memory associated with the ParticleForceGenerator.
	 *
	 * 	@param forceGenerator 	pointer to ParticleForceGenerator to delete.
	 */
	void (*delete)(ParticleForceGenerator *forceGenerator);
} ParticleForceGeneratorManager;

extern const ParticleForceGeneratorManager manParticleForceGenerator;

#endif
