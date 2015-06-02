#include "GameObjectRegistry.h"

#include "col/CollisionResolver.h"
#include "col/CollisionDetection.h"

GameObjectRegist* new(MatrixManager* matMan) {
	GameObjectRegist* regist = malloc(sizeof(GameObjectRegist));

	regist->matMan = matMan;
	regist->gameObjects = manDynamicArray.new(1, sizeof(GameObject*));
	regist->pfRegistry = manForceRegistry.new();
	regist->collisionResolver = manColResolver.new();

	return regist;
}

void add(GameObjectRegist* regist, GameObject* gameObject) {
	gameObject->pfRegistry = regist->pfRegistry;
	manDynamicArray.append(regist->gameObjects, &gameObject);
	manColResolver.addCollider(regist->collisionResolver, gameObject->physCollider);
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

	manColResolver.reset(regist->collisionResolver);

	int i = 0;
	bool flag = true;
	while(flag && i < 10) {
		manColResolver.prepare(regist->collisionResolver);
		flag = manColResolver.check(regist->collisionResolver);

		if (flag)
			manColResolver.resolve(regist->collisionResolver);

		i++;
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
	for(int i = 0; i < regist->gameObjects->size; i++) {
		manGameObj.delete((GameObject*)manDynamicArray.get(regist->gameObjects, i));
	}
	manDynamicArray.delete(regist->gameObjects);
	free(regist->gameObjects);

	manMatMan.delete(regist->matMan);
	free(regist->matMan);

	manColResolver.delete(regist->collisionResolver);
	free(regist->collisionResolver);

	manForceRegistry.delete(regist->pfRegistry);
	free(regist->pfRegistry);
}

const GameObjectRegistManager manGameObjRegist = {new, add, setShader, setMatrixManager, getGameObject, update, render, delete};
