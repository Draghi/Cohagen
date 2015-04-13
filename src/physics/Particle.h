/**
 *	The design of this physics system is described in "Game Physics Engine Development" 
 * 	by Ian Millington (published: 7th March, 2007). Full credit goes to him for the design of this system.
 */
#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>

#include "math/Vec3.h"

typedef struct Particle_s {
	/**
	 *	Position of particle in world space.
	 */
	Vec3 	position;

	/**
	 *	Linear velocity of particle in world space.
	 */
	Vec3 	velocity;

	/**
	 *	Acceleration of particle.
	 */
	Vec3 	acceleration;

	/**
	 *	Damping to apply to motion.
	 * 	Required in order to combat floating-point inaccuracies.
	 */
	scalar 	damping;

	/** 
	 *	Inverse mass of the particle.
	 * 	We use inverse mass because we may want to simulate objects of
	 * 	infinite mass but never want to simulate objects of zero mass 
	 * 	(a = (1/m) * f, if m is zero, acceleration is infinite - bad).
	 * 	If we want to simulate objects of infinite mass, storing the mass
	 * 	isn't feasible as we can't represent infinity very well on a computer.
	 * 	Therefore we use inverse mass (for infinite mass, the inverse mass is 0).
	 */
	scalar inverseMass;

	/**
	 *	Resultant force to apply to an object at each step.
	 * 	Value is reset after each integration step.
	 */
	Vec3 	forceAccum;
} Particle;

typedef struct ParticleManager_s {
	/** 
	 *	Allocate new memory for a Particle instance and return a pointer
	 * 	to that memory.
	 *
	 * 	The particle has the following default values:
	 * 		position = (0, 0, 0)
	 * 		velocity = (0, 0, 0)
	 * 		acceleration = (0, 0, 0)
	 * 		damping = 0.1f
	 * 		inverseMass = 1.0f
	 * 		forceAccum = (0, 0, 0)
	 *
	 * 	@returns 	pointer to Particle, the address of the Particle object on the heap.
	 */
	Particle *(*new)();

	/**
	 *	Frees all memory associated with the given Particle object.
	 *
	 * 	@param 	particle 	pointer to Particle to delete.
	 */
	void (*delete)(Particle *particle);
	/** 
	 *	Updates the particle.
	 *
	 * 	@pre 				frameTime must be grater than 0.
	 *
	 * 	@param 	particle 	const pointer to Particle, the particle to update.
	 * 	@param 	frameTime 	scalar, how long the previous frame took to update.
	 */
	void (*integrate)(Particle *const particle, scalar frameTime);

	/**
	 *	Clear the forces to be applied to a particle at each step.
	 *
	 * 	@param 	particle 	const pointer to Particle who's force accumulator you wish to clear.
	 */
	void (*clearForceAccumulator)(Particle *const particle);

	/**
	 *	Add a force to be applied to the given particle at the
	 * 	next step. Will only be applied at the next step as forces
	 * 	are cleared after each step.
	 * 
	 * 	@param 	particle 	const pointer to Particle, particle to add force to.
	 * 	@param 	force 		const pointer to const Vec3, force to add.
	 */
	void (*addForce)(Particle *const particle, const Vec3 *const force);

	/**
	 *	Get the mass of the given particle.
	 *
	 * 	@pre 				Particle does not have infinite mass.
	 * 
	 * 	@param 	particle 	const pointer to Particle to get the mass of.
	 * 	@returns 			scalar, mass of the particle.
	 */	
	scalar (*getMass)(Particle *const particle);

	/**
	 *	Check if the particle has finite mass.	
	 *
	 * 	@param 	 	particle 	const pointer to Particle, particle to check.
	 * 	@returns 				bool, true if particle has finite mass, false otherwise.
	 */
	bool (*hasFiniteMass)(Particle *const particle);
} ParticleManager;

/**
 *	Because adding a force to a particle using "addForce" only applies
 * 	a force to the next physics step, we need to use force generators
 * 	that generate an appropriate force for each step if we want forces
 * 	to be applied over a longer time period.
 */

extern const ParticleManager manParticle;

#endif