#ifndef COH_SAT_H
#define COH_SAT_H

#include <stdbool.h>
#include "math/Vec3.h"

typedef struct SATProjection_s {
	Vec3 axis;
	scalar min;
	scalar max;
	Vec3 pntMin;
	Vec3 pntMax;
} SATProjection;

typedef struct SAToverlap_s {
	bool isTouching;
	bool isOverlaping;
	bool isContaining;
	bool isExactMatch;
	Vec3 axis;
	scalar push;
	scalar min;
	scalar max;
	Vec3 pntMax;
	Vec3 pntMin;
} SATOverlap;

typedef struct SATSphere_s {
	Vec3 center;
	scalar radius;
} SATSphere;

typedef struct SATMesh_s {
	Vec3* verts;
	int vCount;
	Vec3* norms;
	int nCount;
} SATMesh;

typedef struct SATManager_s {
	void(* projectSphere)(SATProjection* proj, SATSphere* sphere);
	void(* projectMesh)(SATProjection* proj, SATMesh* mesh);
	SATOverlap(* overlap)(SATProjection* p1, SATProjection* p2);
} SATManager;

extern const SATManager sat;

#endif