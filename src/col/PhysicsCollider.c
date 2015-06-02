#include "col/PhysicsCollider.h"

static PhysicsCollider* new(Vec3* position, Vec3* rotation, Vec3* scale, Vec3* velocity, scalar* inverseMass) {
	PhysicsCollider* result = malloc(sizeof(PhysicsCollider));

	if (position != NULL) {
		result->position = position;
	} else {
		result->position = malloc(sizeof(Vec3));
		manVec3.create(result->position, 0, 0, 0);
	}

	if (rotation != NULL)
		result->rotation = rotation;
	else {
		result->rotation = malloc(sizeof(Vec3));
		manVec3.create(result->rotation, 0, 0, 0);
	}

	if (scale != NULL)
		result->scale = scale;
	else {
		result->scale = malloc(sizeof(Vec3));
		manVec3.create(result->scale, 1, 1, 1);
	}

	if (velocity != NULL)
		result->velocity = velocity;
	else {
		result->velocity = malloc(sizeof(Vec3));
		manVec3.create(result->velocity, 0, 0, 0);
	}

	if (inverseMass != NULL)
		result->inverseMass = inverseMass;
	else {
		result->inverseMass = malloc(sizeof(scalar));
		result->inverseMass = 1;
	}

	result->bPhase.center = manVec3.create(NULL, 0,0,0);
	result->bPhase.radius = 0;

	return result;
}

static void setBroadphase(PhysicsCollider* target, Vec3* offset, scalar radius) {
	if (offset != NULL)
		target->bPhase.center = *offset;
	else {
		manVec3.create(&target->bPhase.center, 0, 0, 0);
	}

	target->bPhase.radius = radius;
}

static void attachNarrowphaseSimpleMesh(PhysicsCollider* target, ColliderSimpleMesh* mesh) {
	target->nPhase = *mesh;
}

const PhysicsColliderManager manPhysCollider = {new, setBroadphase, attachNarrowphaseSimpleMesh};
