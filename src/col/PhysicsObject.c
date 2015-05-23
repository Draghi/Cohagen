#include "PhysicsObject.h"

static PhysicsInfo* new(Vec3* position, Vec3* rotation, Vec3* scale, Vec3* velocity) {
	PhysicsInfo* result = malloc(sizeof(PhysicsInfo));

	if (position != NULL)
		result->position = position;
	else {
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

	result->nPhase.collider = NULL;
	result->nPhase.type = COL_TYPE_NONE;
	result->bPhase.center = manVec3.create(NULL, 0,0,0);
	result->bPhase.radius = 0;

	return result;
}

static void setBroadphase(PhysicsInfo* target, Vec3* offset, scalar radius) {
	if (offset != NULL)
		target->bPhase.center = *offset;
	else {
		manVec3.create(&target->bPhase.center, 0, 0, 0);
	}

	target->bPhase.radius = radius;
}

static void attachNarrowphaseSphere(PhysicsInfo* target, Vec3* offset, scalar radius) {
	target->nPhase.type = COL_TYPE_SPHERE;

	if (target->nPhase.collider == NULL)
		target->nPhase.collider = malloc(sizeof(ColliderSphere));

	manVec3.createFromVec3(&((ColliderSphere*)target->nPhase.collider)->center, offset);
	((ColliderSphere*)target->nPhase.collider)->radius = radius;
}

static void attachNarrowphaseSimpleMesh(PhysicsInfo* target, ColliderSimpleMesh* mesh) {
	target->nPhase.type = COL_TYPE_SIMPLE_MESH;
	target->nPhase.collider = mesh;
}

static void attachNarrowphaseComplexMesh(PhysicsInfo* target, ColliderComplexMesh* mesh) {
	target->nPhase.type = COL_TYPE_COMPLEX_MESH;
	target->nPhase.collider = mesh;
}

const PhysicsObjectManager manPhysObj = {new, setBroadphase, attachNarrowphaseSphere, attachNarrowphaseSimpleMesh, attachNarrowphaseComplexMesh};
