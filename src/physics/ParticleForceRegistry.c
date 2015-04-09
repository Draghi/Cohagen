#include "ParticleForceRegistry.h"

static ParticleForceRegistry *new() {
	DynamicArray *forceRegistrations = malloc(sizeof(DynamicArray));
	newDynamicArray(forceRegistrations, 10, sizeof(ParticleForceRegistration));

	ParticleForceRegistry *registry = malloc(sizeof(ParticleForceRegistry));
	registry->forceRegistrations = *forceRegistrations;

	return (registry);
}

static void delete(ParticleForceRegistry *registry) {
	freeDynamicArrayContents(&(registry->forceRegistrations));
	deleteDynamicArray(&(registry->forceRegistrations));
}

static void add(ParticleForceRegistry *const registry, Particle *const particle, ParticleForceGenerator *const forceGenerator) {
	ParticleForceRegistration *registration = malloc(sizeof(ParticleForceRegistration));
	registration->particle = particle;
	registration->forceGenerator = forceGenerator;

	registry->forceRegistrations.append(&(registry->forceRegistrations), registration);
}

static void remove(ParticleForceRegistry *const registry, Particle *const particle, const ParticleForceGenerator *const forceGenerator) {

}

static void updateForces(const ParticleForceRegistry *const registry, scalar frameTime) {
	// Call update force for each registration
	for(int i = 0; i < registry->forceRegistrations.size; ++i) {
		ParticleForceRegistration *registration = (ParticleForceRegistration *) registry->forceRegistrations.get(&(registry->forceRegistrations), i);
		registration->forceGenerator->updateForce(registration->forceGenerator, registration->particle, frameTime);
	}
}


const ParticleForceRegistryManager manForceRegistry = {new, delete, add, remove, updateForces};
