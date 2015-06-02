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
	bool(* checkStaticBroadphase)(ColliderSphere* obj1, ColliderSphere* obj2);
	CollisionResult(* checkStaticNarrowphase)(ColliderSimpleMesh* obj1, ColliderSimpleMesh* obj2);
} CollisionDetectionManager;

extern const CollisionDetectionManager manColDetection;


#endif
