#ifndef COH_COLLISIONDETECTION_H
#define COH_COLLISIONDETECTION_H

#include <stdbool.h>
#include "math/Vec3.h"
#include "col/CollisionMesh.h"
#include "col/PhysicsObject.h"

typedef struct CollisionResult_s {
	Vec3 axis;
	bool isTouching;
	bool isColliding;
	bool isContained;
	bool isMatch;
	scalar distance;
} CollisionResult;

typedef struct CollisionDetectionManager_s {
	bool(* checkStaticBroadphase)(PhysicsInfo* obj1, PhysicsInfo* obj2);
	CollisionResult(* checkStaticNarrowphase)(PhysicsInfo* obj1, PhysicsInfo* obj2);
} CollisionDetectionManager;

extern const CollisionDetectionManager manColDetection;


#endif
