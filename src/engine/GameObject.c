#include "GameObject.h"

#include "render/Renderer.h"

static GameObject* new(char* name, void* parent, bool hasPhysics, bool hasRender, FuncOnUpdate* onUpdateCallback, FuncOnCollide* onCollideCallback, FuncOnRender* onRenderCallback, ParticleForceRegistry* pfRegistry) {
	GameObject* gameObject = malloc(sizeof(GameObject));
	gameObject->name = name;
	gameObject->parent = parent;

	gameObject->position = manVec3.create(NULL, 0,0,0);
	gameObject->rotation = manVec3.create(NULL, 0,0,0);
	gameObject->scale = manVec3.create(NULL, 1,1,1);
	gameObject->velocity = manVec3.create(NULL, 0,0,0);
	gameObject->acceleration = manVec3.create(NULL, 0,0,0);
	gameObject->force = manVec3.create(NULL, 0,0,0);

	if (hasPhysics) {
		gameObject->particle = manParticle.new(&gameObject->position, &gameObject->velocity, &gameObject->acceleration, &gameObject->force);
		gameObject->physCollider = manPhysCollider.new(&gameObject->position, &gameObject->rotation, &gameObject->scale, &gameObject->velocity, &gameObject->particle->inverseMass);
	} else {
		gameObject->physCollider = NULL;
		gameObject->particle = NULL;
	}

	if (hasRender) {
		gameObject->render = manRenderObj.new(&gameObject->position, &gameObject->rotation, &gameObject->scale);
	} else {
		gameObject->render = NULL;
	}

	gameObject->onUpdateCallback = onUpdateCallback;
	gameObject->onCollideCallback = onCollideCallback;
	gameObject->onRenderCallback = onRenderCallback;

	gameObject->pfRegistry = pfRegistry;

	return gameObject;
}

//Functions
static void update(GameObject* gameObject, float tickDelta) {
	if (gameObject->onUpdateCallback != NULL)
		gameObject->onUpdateCallback(gameObject, tickDelta);
}

static void collide(GameObject* gameObject, GameObject* other) {
	if (gameObject->onCollideCallback != NULL)
		gameObject->onCollideCallback(gameObject, other);
}

static void render(GameObject* gameObject, float frameDelta, Shader* shader, MatrixManager* matMan) {
	if (gameObject->onRenderCallback != NULL)
		gameObject->onRenderCallback(gameObject, frameDelta, shader, matMan);

	if (gameObject->render!=NULL) {
		manRenderer.renderModel(gameObject->render, shader, matMan);
	}
}

//Internals
static void setPhysicsCollider(GameObject* gameObject, PhysicsCollider* collider) {
	if (gameObject->physCollider!=NULL) {
		gameObject->physCollider->bPhase = collider->bPhase;
		gameObject->physCollider->nPhase = collider->nPhase;
	}
}

static void setModel(GameObject* gameObject, RenderObject* renderObject) {
	if (gameObject->render!=NULL) {
		gameObject->render->model = renderObject->model;
		gameObject->render->textures = renderObject->textures;
	}
}

//Positioning in space
static void setPositionXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->position.x = x;
	gameObject->position.y = y;
	gameObject->position.z = z;
}

static void addPositionXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->position.x += x;
	gameObject->position.y += y;
	gameObject->position.z += z;
}

static void setPositionVec(GameObject* gameObject, Vec3* newPosition) {
	setPositionXYZ(gameObject, newPosition->x, newPosition->y, newPosition->z);
}

static void addPositionVec(GameObject* gameObject, Vec3* displacement) {
	addPositionXYZ(gameObject, displacement->x, displacement->y, displacement->z);
}

static void setScaleXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->scale.x = x;
	gameObject->scale.y = y;
	gameObject->scale.z = z;
}

static void addScaleXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->scale.x += x;
	gameObject->scale.y += y;
	gameObject->scale.z += z;
}

static void setScaleVec(GameObject* gameObject, Vec3* scale) {
	setScaleXYZ(gameObject, scale->x, scale->y, scale->z);
}

static void addScaleVec(GameObject* gameObject, Vec3* scale) {
	addScaleXYZ(gameObject, scale->x, scale->y, scale->z);
}

static void setRotationXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->rotation.x = x;
	gameObject->rotation.y = y;
	gameObject->rotation.z = z;
}

static void addRotationXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->rotation.x += x;
	gameObject->rotation.y += y;
	gameObject->rotation.z += z;
}

static void setRotationVec(GameObject* gameObject, Vec3* rotation) {
	setRotationXYZ(gameObject, rotation->x, rotation->y, rotation->z);
}

static void addRotationVec(GameObject* gameObject, Vec3* rotation) {
	addRotationXYZ(gameObject, rotation->x, rotation->y, rotation->z);
}

//Physics
static void setVelocityXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->velocity.x = x;
	gameObject->velocity.y = y;
	gameObject->velocity.z = z;
}

static void addVelocityXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->velocity.x += x;
	gameObject->velocity.y += y;
	gameObject->velocity.z += z;
}

static void setVelocityVec(GameObject* gameObject, Vec3* velocity) {
	setVelocityXYZ(gameObject, velocity->x, velocity->y, velocity->z);
}

static void addVelocityVec(GameObject* gameObject, Vec3* velocity) {
	addVelocityXYZ(gameObject, velocity->x, velocity->y, velocity->z);
}

static void setAccelerationXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->acceleration.x = x;
	gameObject->acceleration.y = y;
	gameObject->acceleration.z = z;
}

static void addAccelerationXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->acceleration.x += x;
	gameObject->acceleration.y += y;
	gameObject->acceleration.z += z;
}
static void setAccelerationVec(GameObject* gameObject, Vec3* acceleration)  {
	setAccelerationXYZ(gameObject, acceleration->x, acceleration->y, acceleration->z);
}
static void addAccelerationVec(GameObject* gameObject, Vec3* acceleration) {
	addAccelerationXYZ(gameObject, acceleration->x, acceleration->y, acceleration->z);
}

static void setForceXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->force.x = x;
	gameObject->force.y = y;
	gameObject->force.z = z;
}

static void addForceXYZ(GameObject* gameObject, scalar x, scalar y, scalar z) {
	gameObject->force.x += x;
	gameObject->force.y += y;
	gameObject->force.z += z;
}

static void setForceVec(GameObject* gameObject, Vec3* force) {
	setForceXYZ(gameObject, force->x, force->y, force->z);
}

static void addForceVec(GameObject* gameObject, Vec3* force) {
	addForceXYZ(gameObject, force->x, force->y, force->z);
}

static void addForceGenerator(GameObject* gameObject, ParticleForceGenerator* forceGenerator) {
	manForceRegistry.add(gameObject->pfRegistry, gameObject->particle, forceGenerator);
}

static void delete(GameObject* gameObject) {

}

const GameObjectManager manGameObj = {new, update, collide, render, setPhysicsCollider, setModel, setPositionXYZ, addPositionXYZ, setPositionVec, addPositionVec, setScaleXYZ, addScaleXYZ, setScaleVec, addScaleVec, setRotationXYZ, addRotationXYZ, setRotationVec, addRotationVec, setVelocityXYZ, addVelocityXYZ, setVelocityVec, addVelocityVec, setAccelerationXYZ, addAccelerationXYZ, setAccelerationVec, addAccelerationVec, setForceXYZ, addForceXYZ, setForceVec, addForceVec, addForceGenerator, delete};
