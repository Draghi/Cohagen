#ifndef COH_PHYSICSOBJECT_H
#define COH_PHYSICSOBJECT_H

#include "math/Vec3.h"
#include "CollisionMesh.h"

typedef enum COL_TYPE_E {
	COL_TYPE_NONE,
	COL_TYPE_SPHERE,
	COL_TYPE_SIMPLE_MESH,
	COL_TYPE_COMPLEX_MESH
} COL_TYPE;

typedef struct PhysicsCollider_s {
	Vec3* position;
	Vec3* rotation;
	Vec3* scale;
	Vec3* velocity;
	scalar* inverseMass;

	ColliderSphere bPhase;
	ColliderSimpleMesh nPhase;
} PhysicsCollider;

typedef struct PhysicsColliderManager_s {
	PhysicsCollider*(* new)(Vec3* position, Vec3* rotation, Vec3* scale, Vec3* velocity, scalar* inverseMass);
	void(* setBroadphase)(PhysicsCollider* target, Vec3* offset, scalar radius);
	void(* attachNarrowphaseSimpleMesh)(PhysicsCollider* target, ColliderSimpleMesh* mesh);
} PhysicsColliderManager;

extern const PhysicsColliderManager manPhysCollider;

#endif
