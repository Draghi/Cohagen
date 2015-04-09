#include "ParticleForceGenerator.h"

ParticleForceGenerator *new(void (*updateForce)(void *const self, Particle *const particle, scalar frameTime)) {
	ParticleForceGenerator *fg = malloc(sizeof(ParticleForceGenerator));

	fg->updateForce = updateForce;

	return fg;
}

void delete(ParticleForceGenerator *forceGenerator) {
	free(forceGenerator);
}

const ParticleForceGeneratorManager manParticleForceGenerator = {new, delete};
