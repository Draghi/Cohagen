#include "SAT.h"

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "math/Precision.h"
#include "math/Vec3.h"

static void satProjectPoint(SATProjection* proj, Vec3* point) {
	scalar dp;

	dp = manVec3.dot(&proj->axis, point);

	if (dp<proj->min) {
		proj->min = dp;
		proj->pntMin = *point;
	}

	if (dp>proj->max) {
		proj->max = dp;
		proj->pntMax = *point;
	}
}

static void satProjectSphere(SATProjection* proj, SATSphere* sphere) {
	scalar dot = manVec3.dot(&proj->axis, &sphere->center);
	proj->min = dot - fabs(sphere->radius);
	proj->max = dot + fabs(sphere->radius);

	Vec3 pnt = manVec3.preMulScalar(sphere->radius, &proj->axis);
	proj->pntMin = manVec3.sub(&sphere->center, &pnt);
	proj->pntMax = manVec3.sum(&sphere->center, &pnt);
}

static void satProjectMesh(SATProjection* proj, SATMesh* mesh) {
	int minID = 0;
	int maxID = 0;
	Vec3* currentPoint;
	scalar dp;

	for(int i = 0; i < mesh->vCount; i++) {
		currentPoint = &mesh->verts[i];
		dp = manVec3.dot(&proj->axis, currentPoint);

		if (dp<proj->min) {
			proj->min = dp;
			minID = i;
		}

		if (dp>proj->max) {
			proj->max = dp;
			maxID = i;
		}
	}

	proj->pntMin = mesh->verts[minID];
	proj->pntMax = mesh->verts[maxID];
}

static SATOverlap satOverlap(SATProjection* p1, SATProjection* p2){
	assert(p1->axis.x == p2->axis.x && "SAT Axis Mismatch.");
	assert(p1->axis.y == p2->axis.y && "SAT Axis Mismatch.");
	assert(p1->axis.z == p2->axis.z && "SAT Axis Mismatch.");
	assert(p1->min <= p1->max && "SAT Min/Max Backwards.");
	assert(p2->min <= p2->max && "SAT Min/Max Backwards.");

	SATOverlap ol;
	ol.axis = p1->axis;
	ol.isTouching = false;
	ol.isOverlaping = false;
	ol.isContaining = false;
	ol.isExactMatch = false;

	if ((p1->min < p2->min) && (p1->max < p2->min)) {
		ol.min = p1->min;
		ol.max = p2->max;
		ol.pntMax = p1->pntMin;
		ol.pntMin = p2->pntMax;
	} else if ((p2->min < p1->min) && (p2->max < p1->min)) {
		ol.min = p1->max;
		ol.max = p2->min;
		ol.pntMax = p1->pntMax;
		ol.pntMin = p2->pntMin;
	} else  if (p1->max == p2->min){
		ol.isTouching = true;
		ol.min = p1->max;
		ol.max = p2->min;
		ol.pntMin = p1->pntMax;
		ol.pntMax = p2->pntMin;
	} else if (p2->max == p1->min) {
		ol.isTouching = true;
		ol.min = p1->min;
		ol.max = p2->max;
		ol.pntMin = p1->pntMin;
		ol.pntMax = p2->pntMax;
	} else if (((p1->min >= p2->min) && (p1->min < p2->max)) && !((p1->max <= p2->max) && (p1->max > p2->min))){
		ol.isTouching = true;
		ol.isOverlaping = true;
		ol.min = p1->min;
		ol.max = p2->max;
		ol.pntMin = p1->pntMin;
		ol.pntMax = p2->pntMax;
	} else if (((p2->min >= p1->min) && (p2->min < p1->max)) && !((p2->max <= p1->max) && (p2->max > p1->min))){
		ol.isTouching = true;
		ol.isOverlaping = true;
		ol.min = p1->max;
		ol.max = p2->min;
		ol.pntMin = p1->pntMax;
		ol.pntMax = p2->pntMin;
	} else {
		if ((p1->min == p2->min) || (p1->max == p2->max)) {
			if ((p1->max == p2->max) && (p1->max == p2->max))
				ol.isExactMatch = true;

			ol.isTouching = true;
			ol.isOverlaping = true;
			ol.isContaining = true;
			ol.min = p1->min;
			ol.max = p2->max;
			ol.pntMin = p1->pntMin;
			ol.pntMax = p2->pntMax;
		} else {
			if (((p1->max < p2->max) && (p1->min > p2->min)) || ((p2->max < p1->max) && (p2->min > p1->min))) {
				ol.isTouching = true;
				ol.isOverlaping = true;
				ol.isContaining = true;
				ol.min = p1->min;
				ol.max = p2->max;
				ol.pntMin = p1->pntMin;
				ol.pntMax = p2->pntMax;
			} else {
				printf("Axis: x - %f, y - %f, z - %f\n", ol.axis.x, ol.axis.y, ol.axis.z);
				printf("   Mesh1 Min: %.32f\n", p1->min);
				printf("   Mesh1 Max: %.32f\n", p1->max);
				printf("   Mesh2 Min: %.32f\n", p2->min);
				printf("   Mesh2 Max: %.32f\n", p2->max);
				printf("    Min Diff: %.32f\n", p1->min - p2->min);
				printf("    Max Diff: %.32f\n", p1->max - p2->max);
				printf("Min/Max Diff: %.32f\n", p1->min - p2->max);
				printf("Max/Min Diff: %.32f\n", p1->max - p2->min);
				assert(0 && "SAT INVALID PROJECTION. YELL AT MICHAEL AND GIVE HIM THE DATA ABOVE.");
			}
		}
	}
	ol.push = ol.max - ol.min;

	return ol;
}

const SATManager sat = {satProjectPoint, satProjectSphere, satProjectMesh, satOverlap};
