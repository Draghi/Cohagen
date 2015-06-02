#ifndef COH_COLLISIONMESH_H
#define COH_COLLISIONMESH_H

#include "col/SAT.h"
#include "math/Mat4.h"
#include "math/Vec3.h"

typedef SATSphere ColliderSphere;
typedef struct ColliderSimpleMesh_s {
	SATMesh satMesh;
	int* minPointForAxis;
	int* maxPointForAxis;
}ColliderSimpleMesh;

typedef struct CollisionMeshManager_s {
	ColliderSimpleMesh*(* newSimpleMesh)(int vertCount, Vec3* verts, int normCount, Vec3* norms, int* minPoints, int* maxPoints);
	ColliderSphere*(* newColliderSphere)(Vec3* centerPos, scalar radius);

	Mat4(* makeTransformationMatrix)(Vec3* pos, Vec3* rot, Vec3* scale);
	void(* transformSphere)(SATSphere* sphere, Mat4* matrix, Vec3* vScale);
	ColliderSimpleMesh(* copyMesh)(ColliderSimpleMesh* mesh);
	void(* transformSimpleMesh)(ColliderSimpleMesh* mesh, Mat4* matrix);

	void(* deleteSimpleMesh)(ColliderSimpleMesh* mesh);
	void(* deleteColliderSphere)(ColliderSphere* mesh);
} CollisionMeshManager;

extern const CollisionMeshManager manColMesh;

#endif
