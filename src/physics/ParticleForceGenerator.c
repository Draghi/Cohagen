#include "ParticleForceGenerator.h"

static ParticleForceGenerator *new(void *const self, void (*updateForce)(void *const self, Particle *const particle, scalar frameTime)) {
	ParticleForceGenerator *fg = malloc(sizeof(ParticleForceGenerator));

	fg->self = self;
	fg->updateForce = updateForce;

	return fg;
}

static void delete(ParticleForceGenerator *forceGenerator) {
	free(forceGenerator);
}

const ParticleForceGeneratorManager manParticleForceGenerator = {new, delete};
