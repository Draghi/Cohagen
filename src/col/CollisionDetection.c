#include "CollisionDetection.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "col/SAT.h"
#include "math/Mat4.h"

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
	for(int i = 0; i < mesh1->satMesh.nCount; i++) {
		SATProjection mesh1Proj, mesh2Proj;
		mesh1Proj.axis = mesh1->satMesh.norms[i];
		mesh1Proj.min = SCALAR_MAX_VAL;
		mesh1Proj.max = SCALAR_MIN_VAL;

		mesh2Proj.axis = mesh1->satMesh.norms[i];
		mesh2Proj.min = SCALAR_MAX_VAL;
		mesh2Proj.max = SCALAR_MIN_VAL;

		if ((mesh1->minPointForAxis!=NULL) && (mesh1->maxPointForAxis!=NULL)) {
			//If we know the min and max point, then we only need to project them.
			//Significantly reduces calculation complexity.
			sat.projectPoint(&mesh1Proj, &mesh1->satMesh.verts[mesh1->minPointForAxis[i]]);
			sat.projectPoint(&mesh1Proj, &mesh1->satMesh.verts[mesh1->maxPointForAxis[i]]);
		} else {
			//If we don't have the cache data availble, then do the normal calculation.
			sat.projectMesh(&mesh1Proj, &mesh1->satMesh);
		}


		if ((mesh2->minPointForAxis!=NULL) && (mesh2->maxPointForAxis!=NULL)) {
			int j;
			bool matchNormFlag = false;
			for(j = 0; j < mesh2->satMesh.nCount; j++) {
				Vec3* norm = &mesh2->satMesh.norms[j];
				if ((mesh2Proj.axis.x == norm->x) && (mesh2Proj.axis.y == norm->y) && (mesh2Proj.axis.z == norm->z)) {
					matchNormFlag = true;
					break;
				}
			}

			if (matchNormFlag) {
				//If we know the min and max point, then we only need to project them.
				//Significantly reduces calculation complexity.
				sat.projectPoint(&mesh2Proj, &mesh2->satMesh.verts[mesh2->minPointForAxis[j]]);
				sat.projectPoint(&mesh2Proj, &mesh2->satMesh.verts[mesh2->maxPointForAxis[j]]);
			} else {
				//If we don't have the cache data availble, then do the normal calculation.
				sat.projectMesh(&mesh2Proj, &mesh2->satMesh);
			}
		} else {
			//If we don't have the cache data availble, then do the normal calculation.
			sat.projectMesh(&mesh2Proj, &mesh2->satMesh);
		}

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

static bool fastSphereVsSphere(ColliderSphere* bf1, ColliderSphere* bf2) {
	Vec3 disp = manVec3.sub(&bf2->center, &bf1->center);
	scalar sqrDist = disp.x * disp.x + disp.y * disp.y + disp.z * disp.z;
	scalar rad = bf1->radius+bf2->radius;

	sqrDist -= rad*rad;

	return sqrDist<= 0;
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
			if (overlap2.push>overlap.push) {
				overlap = overlap2;
			}
		}
	}

	return satOverlapCollision(&overlap);
}

bool checkStaticBroadphase(ColliderSphere* obj1, ColliderSphere* obj2) {
	return fastSphereVsSphere(obj1, obj2);
}

CollisionResult checkStaticNarrowphase(ColliderSimpleMesh* obj1, ColliderSimpleMesh* obj2) {
	return simpleMeshVsSimpleMesh(obj1, obj2);
}

const CollisionDetectionManager manColDetection = {checkStaticBroadphase, checkStaticNarrowphase};
