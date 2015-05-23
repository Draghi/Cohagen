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

typedef struct NarrowphaseInfo_s {
	COL_TYPE type;
	void* collider;
} NarrowphaseCollider;

typedef struct PhysicsInfo_s {
	Vec3* position;
	Vec3* rotation;
	Vec3* scale;
	Vec3* velocity;

	ColliderSphere bPhase;
	NarrowphaseCollider nPhase;
} PhysicsInfo;

typedef struct PhysicsObjectManager_s {
	PhysicsInfo*(* new)(Vec3* position, Vec3* rotation, Vec3* scale, Vec3* velocity);
	void(* setBroadphase)(PhysicsInfo* target, Vec3* offset, scalar radius);
	void(* attachNarrowphaseSphere)(PhysicsInfo* target, Vec3* offset, scalar radius);
	void(* attachNarrowphaseSimpleMesh)(PhysicsInfo* target, ColliderSimpleMesh* mesh);
	void(* attachNarrowphaseComplexMesh)(PhysicsInfo* target, ColliderComplexMesh* mesh);
} PhysicsObjectManager;

extern const PhysicsObjectManager manPhysObj;

#endif
