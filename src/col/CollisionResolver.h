#ifndef COH_COLLISIONRESPONSE_H
#define COH_COLLISIONRESPONSE_H

#include "col/CollisionDetection.h"
#include "util/DynamicArray.h"

typedef struct CollisionRecord_s {
	int collider1;
	int collider2;
	CollisionResult collisionInfo;
} CollisionRecord;

typedef struct TransformedCollider_s {
	PhysicsCollider collider;
	bool hasMeshTransformed;
} TransformedCollider;

typedef struct CollisionResolver_s {
	DynamicArray* colliders;
	DynamicArray* transformedColliders;
	DynamicArray* collisionRecords;
} CollisionResolver;

typedef struct CollisionResolverManager_s {
	CollisionResolver*(* new)();
	void(* addCollider)(CollisionResolver* collisionResolver, PhysicsCollider* collider);

	void(* reset)(CollisionResolver* collisionResolver);
	void(* prepare)(CollisionResolver* collisionResolver);
	bool(* check)(CollisionResolver* collisionResolver);
	void(* resolve)(CollisionResolver* collisionResolver);

	void(* delete)(CollisionResolver* collisionResolver);
} CollisionResolverManager;

extern const CollisionResolverManager manColResolver;

#endif
