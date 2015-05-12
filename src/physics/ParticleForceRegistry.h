/**
 *	The design of this physics system is described in "Game Physics Engine Development" 
 * 	by Ian Millington (published: 7th March, 2007). Full credit goes to him for the design of this system.
 */
#ifndef PARTICLE_FORCE_REGISTRY_H
#define PARTICLE_FORCE_REGISTRY_H

#include "util/DynamicArray.h"
#include "Particle.h"
#include "ParticleForceGenerator.h"
 
/**
 *	Connection between a particle and a force generator.
 */
typedef struct ParticleForceRegistration_s {
	/**
	 *	Particle to update with "updateForce" function pointer.
	 */
	Particle *particle;

	/**
	 *	Force generator to use to update particle.
	 */
	ParticleForceGenerator *forceGenerator;
} ParticleForceRegistration;

/**
 *	A registry containing an array of ParticleForceRegistrations.
 */
typedef struct ParticleForceRegistry_s {
	DynamicArray 	*forceRegistrations;
} ParticleForceRegistry;

/** 
 *	Manager that allows to manipulate the ParticleForceRegistry.
 */
typedef struct ParticleForceRegistryManager_s {
	/**
	 *	Allocate memory for a new ParticleForceRegistry and return a pointer to it.
	 *
	 * 	@returns 	pointer to ParticleForceRegistry, new ParticleForceRegistry.
	 */
	ParticleForceRegistry *(*new)();

	/**
	 *	Free all memory associated with the given ParticleForceRegistry.
	 *
	 * 	@param 	registry 	pointer to ParticleForceRegistry.
	 */
	void (*delete)(ParticleForceRegistry *registry);

	/**
	 *	Register a connection between the given particle and the given force generator.
	 *
	 * 	@param 	registry 		const pointer to ParticleForceRegistry, the registry to register particle and force function on.
	 * 	@param 	particle 		const pointer to Particle, the particle to update.
	 * 	@param 	forceGenerator 	const pointer to const ParticleForceGenerator, the force generator to use to update the particle.
	 */
	void (*add)(ParticleForceRegistry *const registry, Particle *const particle, ParticleForceGenerator *const forceGenerator);

	/**
	 *	Remove the registration of a connection between the given particle and the given force generator.
	 * 	Has no effect if no registration that matches exists.
	 * 	Does not remove particle or force generator or ParticleForceRegistration. Only removes their existence in the registry.
	 */
	void (*remove)(ParticleForceRegistry *const registry, Particle *const particle, const ParticleForceGenerator *const forceGenerator);

	/**
	 *	Update all the force generators in the registry and applies them to their
	 * 	respective particle.
	 *
	 * 	@param 	registry 	const pointer to const ParticleForceRegistry, registry to update forces from.
	 * 	@param 	frametime 	scalar, the duration of the previous frame.
	 */
	void (*updateForces)(const ParticleForceRegistry *const registry, scalar frameTime);
} ParticleForceRegistryManager;

extern const ParticleForceRegistryManager manForceRegistry;

#endif
