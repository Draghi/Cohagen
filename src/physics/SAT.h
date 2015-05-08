#ifndef COH_SAT_H
#define COH_SAT_H

#include <stdbool.h>
#include "math/Vec3.h"

typedef struct SATInfo_s {
	bool isColliding;
	Vec3 axis;
	float distance;
} SATInfo;

typedef struct SATSphere_s {
	Vec3 center;
	float radius;
} SATSphere;

typedef struct SATPlane_s {
	Vec3 position;
	Vec3 normal;
} SATPlane;

typedef struct SATPlaneMesh_s {
	SATPlane* planes;
	int count;
} SATPlaneMesh;

SATInfo satSphereVsSphere(SATSphere* sphere1, SATSphere* sphere2);
SATInfo satSphereVsPlaneMesh(SATSphere* sphere, SATPlaneMesh* planeMesh);
SATInfo satPlaneMeshVsPlaneMesh(SATPlaneMesh* planeMesh1, SATPlaneMesh* planeMesh2);


#endif
