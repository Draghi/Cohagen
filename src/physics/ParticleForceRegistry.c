#include "ParticleForceRegistry.h"

static ParticleForceRegistry *new() {
	ParticleForceRegistry *registry = malloc(sizeof(ParticleForceRegistry));

	registry->forceRegistrations = manDynamicArray.new(10, sizeof(ParticleForceRegistration));

	return (registry);
}

static void delete(ParticleForceRegistry *registry) {
	manDynamicArray.delete(registry->forceRegistrations);
}

static void add(ParticleForceRegistry *const registry, Particle *const particle, ParticleForceGenerator *const forceGenerator) {
	ParticleForceRegistration registration;
	registration.particle = particle;
	registration.forceGenerator = forceGenerator;

	manDynamicArray.append(registry->forceRegistrations, &registration);
}

static void remove(ParticleForceRegistry *const registry, Particle *const particle, const ParticleForceGenerator *const forceGenerator) {

}

static void updateForces(const ParticleForceRegistry *const registry, scalar frameTime) {
	// Call update force for each registration
	for(int i = 0; i < registry->forceRegistrations->size; ++i) {
		ParticleForceRegistration *registration = (ParticleForceRegistration *) manDynamicArray.get(registry->forceRegistrations, i);
		registration->forceGenerator->updateForce(registration->forceGenerator, registration->particle, frameTime);
	}
}

const ParticleForceRegistryManager manForceRegistry = {new, delete, add, remove, updateForces};
