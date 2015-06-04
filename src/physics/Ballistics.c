#include "stdio.h"

#include "Ballistics.h"
#include "util/ObjLoader.h"

static BallisticSim *new(const Vec3 *const launchPoint) {
	BallisticSim *ballisticSim = malloc(sizeof(BallisticSim));
	BallisticRepresentation *defaultRep = malloc(sizeof(BallisticRepresentation));

	defaultRep->shader = manShader.newFromGroup("./data/shaders/", "passThru");
	int posLoc = manShader.getAttribLocation(defaultRep->shader, "position");
	int normLoc = manShader.getAttribLocation(defaultRep->shader, "normal");
	int texLoc = manShader.getAttribLocation(defaultRep->shader, "texCoord");
	defaultRep->vao = objLoader.genVAOFromFile("./data/models/cube.obj", posLoc, normLoc, texLoc);

	for (int i = 0; i < MAX_NUM_ROUNDS; ++i) {
		ballisticSim->rounds[i].particle = manParticle.new(NULL, NULL, NULL, NULL);
		ballisticSim->rounds[i].type = UNUSED;
		ballisticSim->rounds[i].representation = defaultRep;
	}

	return ballisticSim;
}

static void delete(BallisticSim *ballisticSim)
{
	for (int i = 0; i < MAX_NUM_ROUNDS; ++i) {
		manParticle.delete(ballisticSim->rounds[i].particle);
	}
}

static void renderAll(BallisticSim *const ballisticSim) {
	for (int i = 0; i < MAX_NUM_ROUNDS; ++i) {
		if (ballisticSim->rounds[i].type != UNUSED) {
			ballisticSim->rounds[i].render(&(ballisticSim->rounds[i]), ballisticSim->rounds[i].representation);
		}
	}	
}

static void update(BallisticSim *const ballisticSim) {

}

static void fire(BallisticSim *const ballisticSim) {

}


const BallisticManager manBallistic = {new, delete, renderAll, update, fire};
