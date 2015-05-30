#ifndef COH_COLLISIONDETECTION_H
#define COH_COLLISIONDETECTION_H

#include "col/PhysicsCollider.h"
#include <stdbool.h>
#include "math/Vec3.h"
#include "col/CollisionMesh.h"

typedef struct CollisionResult_s {
	Vec3 axis;
	bool isTouching;
	bool isColliding;
	bool isContained;
	bool isMatch;
	scalar distance;
} CollisionResult;

typedef struct CollisionDetectionManager_s {
	bool(* checkStaticBroadphase)(PhysicsCollider* obj1, PhysicsCollider* obj2);
	CollisionResult(* checkStaticNarrowphase)(PhysicsCollider* obj1, PhysicsCollider* obj2);
} CollisionDetectionManager;

extern const CollisionDetectionManager manColDetection;


#endif
