#include "GameObjectRegistry.h"

#include "col/CollisionDetection.h"
#include "col/CollisionResponse.h"

GameObjectRegist* new(MatrixManager* matMan) {
	GameObjectRegist* regist = malloc(sizeof(GameObjectRegist));

	regist->matMan = matMan;
	regist->gameObjects = manDynamicArray.new(1, sizeof(GameObject*));
	regist->pfRegistry = manForceRegistry.new();

	return regist;
}

void add(GameObjectRegist* regist, GameObject* gameObject) {
	gameObject->pfRegistry = regist->pfRegistry;
	manDynamicArray.append(regist->gameObjects, &gameObject);
}

void setShader(GameObjectRegist* regist, Shader* shader) {
	regist->currentShader = shader;
}

void setMatrixManager(GameObjectRegist* regist, MatrixManager* matMan) {
	regist->matMan = matMan;
}

GameObject* getGameObject(GameObjectRegist* regist, int id) {
	return *((GameObject**) manDynamicArray.get(regist->gameObjects, id));
}

void update(GameObjectRegist* regist, float tickDelta) {

	//Call update functions
	for(unsigned int i = 0; i <regist->gameObjects->size; i++) {
		GameObject* gameObject = getGameObject(regist, i);
		manGameObj.update(gameObject, tickDelta);
	}

	manForceRegistry.updateForces(regist->pfRegistry, tickDelta);

	//Integrate particles
	for(unsigned int i = 0; i < regist->gameObjects->size; i++) {
		GameObject* gameObject = getGameObject(regist, i);
		if (gameObject->particle!=NULL)
			manParticle.integrate(gameObject->particle, tickDelta);
	}

	//Do Collisions
	for(int i = 0; i < regist->gameObjects->size; i++) {
		for(int j = i+1; j < regist->gameObjects->size; j++) {
			GameObject* g1 = getGameObject(regist, i);
			GameObject* g2 = getGameObject(regist, j);

			if ((g1->physCollider != NULL) && (g1->physCollider != NULL)) {
				if (manColDetection.checkStaticBroadphase(g1->physCollider, g2->physCollider)) {
					CollisionResult info = manColDetection.checkStaticNarrowphase(g1->physCollider, g2->physCollider);
					if (info.isColliding) {
						Vec3 translation = manVec3.preMulScalar(info.distance/2, &info.axis);

						*g1->physCollider->position = manVec3.sum(g1->physCollider->position, &translation);
						translation = manVec3.invert(&translation);
						*g2->physCollider->position = manVec3.sum(g2->physCollider->position, &translation);

						if ((g1->particle != NULL) && (g2->particle != NULL)) {
							momentumCollisionResponse(g1->particle->velocity, g2->particle->velocity,
													  *g1->particle->velocity, *g2->particle->velocity,
													  manParticle.getMass(g1->particle), manParticle.getMass(g2->particle));
						} else if (g1->particle != NULL) {
							momentumCollisionResponse(g1->physCollider->velocity, g2->particle->velocity,
													  *g1->physCollider->velocity, *g2->particle->velocity,
													 SCALAR_MAX_VAL, manParticle.getMass(g2->particle));
						} else if (g2->particle != NULL) {
							momentumCollisionResponse(g1->particle->velocity, g2->physCollider->velocity,
													  *g1->particle->velocity, *g2->physCollider->velocity,
													  manParticle.getMass(g1->particle), SCALAR_MAX_VAL);
						} else {
							momentumCollisionResponse(g1->particle->velocity, g2->particle->velocity,
													  *g1->particle->velocity, *g2->particle->velocity,
													  SCALAR_MAX_VAL, SCALAR_MAX_VAL);
						}

						manGameObj.collide(g1, g2);
						manGameObj.collide(g2, g1);
					}
				}
			}
		}
	}

}

void render(GameObjectRegist* regist, float frameDelta) {
	manMatMan.setMode(regist->matMan, MATRIX_MODE_MODEL);
	for(int i = 0; i < regist->gameObjects->size; i++) {
		GameObject* gameObject = getGameObject(regist, i);
		manGameObj.render(gameObject, frameDelta, regist->currentShader, regist->matMan);
	}
}

void delete(GameObjectRegist* regist) {

}

const GameObjectRegistManager manGameObjRegist = {new, add, setShader, setMatrixManager, getGameObject, update, render, delete};
