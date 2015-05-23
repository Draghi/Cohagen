#include "CollisionDetection.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "col/SAT.h"
#include "math/Mat4.h"

static CollisionResult noCollision = {{0,0,0}, 0, false, false};

static CollisionResult satOverlapCollision(SATOverlap* overlap) {
	CollisionResult result;

	result.axis = overlap->axis;
	result.distance = overlap->push;
	result.isTouching = overlap->isTouching;
	result.isColliding = overlap->isOverlaping;
	result.isContained = overlap->isContaining;
	result.isMatch = overlap->isExactMatch;

	return result;
}

static bool doSimpleMeshvsSimpleMesh(SATOverlap* result, ColliderSimpleMesh* mesh1, ColliderSimpleMesh* mesh2) {
	for(int i = 0; i < mesh1->nCount; i++) {
		SATProjection mesh1Proj, mesh2Proj;
		mesh1Proj.axis = mesh1->norms[i];
		mesh1Proj.min = SCALAR_MAX_VAL;
		mesh1Proj.max = SCALAR_MIN_VAL;

		mesh2Proj.axis = mesh1->norms[i];
		mesh2Proj.min = SCALAR_MAX_VAL;
		mesh2Proj.max = SCALAR_MIN_VAL;

		sat.projectMesh(&mesh1Proj, mesh1);
		sat.projectMesh(&mesh2Proj, mesh2);

		SATOverlap overlap = sat.overlap(&mesh1Proj, &mesh2Proj);

		if(!overlap.isTouching) {
			*result = overlap;
			return false;
		} else {
			if (fabs(overlap.push) < fabs(result->push)) {
				*result = overlap;
			}
		}
	}

	return true;
}

static bool doSimpleMeshvsSphere(SATOverlap* result, ColliderSimpleMesh* mesh, ColliderSphere* sphere) {
	for(int i = 0; i < mesh->nCount; i++) {
		SATProjection sphereProj, meshProj;
		sphereProj.axis = mesh->norms[i];
		sphereProj.min = SCALAR_MAX_VAL;
		sphereProj.max = SCALAR_MIN_VAL;

		meshProj.axis = mesh->norms[i];
		meshProj.min = SCALAR_MAX_VAL;
		meshProj.max = SCALAR_MIN_VAL;

		sat.projectSphere(&sphereProj, sphere);
		sat.projectMesh(&meshProj, mesh);

		SATOverlap overlap = sat.overlap(&sphereProj, &meshProj);
		if(!overlap.isTouching) {
			*result = overlap;
			return false;
		} else {
			if (fabs(overlap.push) < fabs(result->push)) {
				*result = overlap;
			}
		}
	}

	return true;
}

static bool doSpherevsSimpleMesh(SATOverlap* result, ColliderSphere* sphere, ColliderSimpleMesh* mesh) {
	int axisCount = mesh->vCount;
	Vec3* axis = malloc(sizeof(Vec3)*axisCount);

	int j = 0;
	for(int i = 0; i < axisCount; i++) {
		Vec3 displacement = manVec3.sub(&mesh->verts[i], &sphere->center);
		//Only add actual axis we need to test.
		//Points matching the center have been picked up by the previous loop and don't need to be re-tested.
		if (manVec3.magnitude(&displacement) > 0) {
			axis[j] = manVec3.normalize(&displacement);
			j++;
		}
	}

	axisCount = j;

	for(int i = 0; i < axisCount; i++) {
		SATProjection sphereProj, meshProj;
		sphereProj.axis = axis[i];
		sphereProj.min = SCALAR_MAX_VAL;
		sphereProj.max = SCALAR_MIN_VAL;

		meshProj.axis = axis[i];
		meshProj.min = SCALAR_MAX_VAL;
		meshProj.max = SCALAR_MIN_VAL;

		sat.projectSphere(&sphereProj, sphere);
		sat.projectMesh(&meshProj, mesh);

		SATOverlap overlap = sat.overlap(&meshProj, &meshProj);

		if(!overlap.isTouching) {
			*result = overlap;
			return false;
		} else {
			if (fabs(overlap.push) < fabs(result->push)) {
				*result = overlap;
			}
		}
	}

	return true;
}

static bool fastSphereVsSphere(ColliderSphere* bf1, ColliderSphere* bf2) {
	Vec3 disp = manVec3.sub(&bf2->center, &bf1->center);
	scalar sqrDist = disp.x * disp.x + disp.y * disp.y + disp.z * disp.z;
	sqrDist -= bf1->radius*bf1->radius + bf2->radius*bf2->radius;

	return sqrDist<= 0;
}

static CollisionResult sphereVsSphere(ColliderSphere* sphere1, ColliderSphere* sphere2) {
	Vec3 disp = manVec3.sub(&sphere2->center, &sphere1->center);
	scalar sqrDist = disp.x * disp.x + disp.y * disp.y + disp.z * disp.z;
	sqrDist = sphere1->radius - sphere2->radius;

	if (sqrDist<= 0) {
		CollisionResult collision;
		collision.distance = sqrt(sqrDist);
		collision.isTouching = true;
		collision.isColliding = sqrDist < 0 ? true : false;

		//Working out the collision axis.
		if (manVec3.magnitude(&disp) > 0) {
			collision.axis = manVec3.normalize(&disp);
		} else {
			collision.axis = manVec3.create(NULL, 0,0,0);
		}
		return collision;
	} else {
		return noCollision;
	}
}

static CollisionResult simpleMeshVsSimpleMesh(ColliderSimpleMesh* mesh1, ColliderSimpleMesh* mesh2) {
	SATOverlap overlap;
	overlap.push = SCALAR_MAX_VAL;

	if (doSimpleMeshvsSimpleMesh(&overlap, mesh1, mesh2)) {
		SATOverlap overlap2;
		overlap2.push = SCALAR_MAX_VAL;

		if (doSimpleMeshvsSimpleMesh(&overlap2, mesh2, mesh1)) {
			overlap2.push = -overlap2.push;
			if (overlap2.push<overlap.push) {
				overlap = overlap2;
			}
		} else {
			overlap2.push = -overlap2.push;
			overlap = overlap2;
		}
	}

	return satOverlapCollision(&overlap);
}

static CollisionResult sphereVsSimpleMesh(ColliderSphere* sphere, ColliderSimpleMesh* mesh) {
	SATOverlap overlap;
	overlap.push = SCALAR_MAX_VAL;
	if (doSimpleMeshvsSphere(&overlap, mesh, sphere)) {
		SATOverlap overlap2;
		overlap2.push = SCALAR_MAX_VAL;

		if (doSpherevsSimpleMesh(&overlap2, sphere, mesh)) {
			if (overlap2.push<overlap.push) {
				overlap = overlap2;
			}
		} else {
			overlap = overlap2;
		}
	}

	return satOverlapCollision(&overlap);
}

bool checkStaticBroadphase(PhysicsInfo* obj1, PhysicsInfo* obj2) {
	Mat4 m1 = manColMesh.makeTransformationMatrix(obj1->position, obj1->rotation, obj1->scale);
	Mat4 m2 = manColMesh.makeTransformationMatrix(obj2->position, obj2->rotation, obj2->scale);

	ColliderSphere bf1 = obj1->bPhase;
	ColliderSphere bf2 = obj2->bPhase;
	manColMesh.transformSphere(&bf1, &m1, obj1->scale);
	manColMesh.transformSphere(&bf2, &m2, obj2->scale);

	return fastSphereVsSphere(&bf1, &bf2);
}

CollisionResult checkStaticNarrowphase(PhysicsInfo* obj1, PhysicsInfo* obj2) {
	Mat4 m1 = manColMesh.makeTransformationMatrix(obj1->position, obj1->rotation, obj1->scale);
	Mat4 m2 = manColMesh.makeTransformationMatrix(obj2->position, obj2->rotation, obj2->scale);

	COL_TYPE t1 = obj1->nPhase.type;
	COL_TYPE t2 = obj2->nPhase.type;

	switch(t1) {
		case COL_TYPE_SPHERE:
			switch(t2) {
				case COL_TYPE_SPHERE: {
					ColliderSphere sphere1 = *((ColliderSphere*)obj1->nPhase.collider);
					ColliderSphere sphere2 = *((ColliderSphere*)obj2->nPhase.collider);
					manColMesh.transformSphere(&sphere1, &m1, obj1->scale);
					manColMesh.transformSphere(&sphere2, &m2, obj2->scale);

					return sphereVsSphere(&sphere1, &sphere2);
				}
				case COL_TYPE_SIMPLE_MESH: {
					ColliderSphere sphere = *((ColliderSphere*)obj1->nPhase.collider);
					ColliderSimpleMesh mesh = manColMesh.copyMesh(obj2->nPhase.collider);
					manColMesh.transformSphere(&sphere, &m1, obj1->scale);
					manColMesh.transformSimpleMesh(&mesh, &m1);

					CollisionResult collision =  sphereVsSimpleMesh(&sphere, &mesh);

					manColMesh.deleteSimpleMesh(&mesh);

					return collision;
				}
				case COL_TYPE_COMPLEX_MESH:
					assert(0 && "ONLY SHPERE VS SPHERE COLLISIONS IMPLEMENTED SO FAR, FOR SPHERES, YELL AT MICHAEL!");
					break;
				case COL_TYPE_NONE:
					break; //Do Nothing
			}
		break;
		case COL_TYPE_SIMPLE_MESH:
			switch(t2) {
				case COL_TYPE_SIMPLE_MESH: {
					ColliderSimpleMesh mesh1 = manColMesh.copyMesh(obj1->nPhase.collider);
					ColliderSimpleMesh mesh2 = manColMesh.copyMesh(obj2->nPhase.collider);
					manColMesh.transformSimpleMesh(&mesh1, &m1);
					manColMesh.transformSimpleMesh(&mesh2, &m2);

					CollisionResult collision = simpleMeshVsSimpleMesh(&mesh1, &mesh2);

					manColMesh.deleteSimpleMesh(&mesh1);
					manColMesh.deleteSimpleMesh(&mesh2);

					return collision;
				}
				case COL_TYPE_SPHERE: {
					ColliderSphere sphere = *((ColliderSphere*)obj2->nPhase.collider);
					ColliderSimpleMesh mesh = manColMesh.copyMesh(obj1->nPhase.collider);
					manColMesh.transformSphere(&sphere, &m1, obj2->scale);
					manColMesh.transformSimpleMesh(&mesh, &m1);

					CollisionResult collision =  sphereVsSimpleMesh(&sphere, &mesh);

					//Reverse the push distance so we know how far it is from obj1 to obj2.
					collision.distance = -collision.distance;

					manColMesh.deleteSimpleMesh(&mesh);

					return collision;
				}
				case COL_TYPE_COMPLEX_MESH:
					assert(0 && "ONLY SIMPLE MESH VS SIMPLE MESH COLLISIONS IMPLEMENTED SO FAR, FOR SIMPLE MESHES, YELL AT MICHAEL!");
					break;
				case COL_TYPE_NONE:
					break; //Do Nothing
			}
		break;
		case COL_TYPE_COMPLEX_MESH:
			switch(t2) {
				case COL_TYPE_SPHERE: //no break
				case COL_TYPE_SIMPLE_MESH: //no break
				case COL_TYPE_COMPLEX_MESH: //no break
					assert(0 && "COMPLEX COLLISIONS NOT IMPLEMENTED YET, YELL AT MICHAEL!");
					break;
				case COL_TYPE_NONE:
					break; //Do Nothing
			}
		break;
		case COL_TYPE_NONE:
			break; //Do Nothing
		default:
			assert(0 && "Invalid collision enum type!");
	}

	//If nothing is returned, return no collision
	return noCollision;
}

const CollisionDetectionManager manColDetection = {checkStaticBroadphase, checkStaticNarrowphase};
