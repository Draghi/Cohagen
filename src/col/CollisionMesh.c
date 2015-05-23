#include "CollisionMesh.h"

static const Vec3 xAxis = {1, 0, 0};
static const Vec3 yAxis = {0, 1, 0};
static const Vec3 zAxis = {0, 0, 1};

static ColliderSimpleMesh* newSimpleMesh(int vertCount, Vec3* verts, int normCount, Vec3* norms) {
	ColliderSimpleMesh* newMesh = malloc(sizeof(ColliderSimpleMesh));

	newMesh->vCount = vertCount;
	newMesh->verts = verts;
	newMesh->nCount = normCount;
	newMesh->norms = norms;

	return newMesh;
}

static ColliderComplexMesh* newComplexMesh(int subMeshCount, ColliderSimpleMesh* subMeshes) {
	ColliderComplexMesh* newMesh = malloc(sizeof(ColliderComplexMesh));

	newMesh->count = subMeshCount;
	newMesh->meshes = subMeshes;

	return newMesh;
}

static ColliderSphere* newColliderSphere(Vec3* centerPos, scalar radius) {
	ColliderSphere* newMesh = malloc(sizeof(ColliderSphere));

	newMesh->center = *centerPos;
	newMesh->radius = radius;

	return newMesh;
}

static Mat4 makeTransformationMatrix(Vec3* pos, Vec3* rot, Vec3* scale) {
	Mat4 matrix = manMat4.createLeading(NULL, 1);

	matrix = manMat4.affTranslate(&matrix, pos);
	matrix = manMat4.affScale(&matrix, scale);
	matrix = manMat4.affRotate(&matrix, rot->x, &xAxis);
	matrix = manMat4.affRotate(&matrix, rot->y, &yAxis);
	matrix = manMat4.affRotate(&matrix, rot->z, &zAxis);

	return matrix;
}

static scalar max(scalar n1, scalar n2) {
	return n1 > n2 ? n1 : n2;
}

static void transformSphere(SATSphere* sphere, Mat4* matrix, Vec3* vScale) {
	Vec4 tmp = manVec4.createFromVec3(NULL, &sphere->center, 1);

	tmp = manMat4.postMulVec4(matrix, &tmp);
	sphere->center.x = tmp.x;
	sphere->center.y = tmp.y;
	sphere->center.z = tmp.z;

	scalar scale = max(max(vScale->x, vScale->y),vScale->z);
	sphere->radius *= scale;
}

static ColliderSimpleMesh copyMesh(ColliderSimpleMesh* mesh) {
	ColliderSimpleMesh dest;
	dest.vCount = mesh->vCount;
	dest.verts = malloc(sizeof(Vec3)*dest.vCount);
	for(int i = 0; i < dest.vCount; i++) {
		dest.verts[i] = mesh->verts[i];
	}

	dest.nCount = mesh->nCount;
	dest.norms = malloc(sizeof(Vec3)*dest.nCount);
	for(int i = 0; i < dest.nCount; i++) {
		dest.norms[i] = mesh->norms[i];
	}

	return dest;
}

static void transformSimpleMesh(ColliderSimpleMesh* mesh, Mat4* matrix) {
	for(int i = 0; i < mesh->vCount; i++) {
		Vec4 tmp = manVec4.createFromVec3(NULL, &mesh->verts[i], 1);
		tmp = manMat4.postMulVec4(matrix, &tmp);

		mesh->verts[i].x = tmp.x;
		mesh->verts[i].y = tmp.y;
		mesh->verts[i].z = tmp.z;
	}

	for(int i = 0; i < mesh->nCount; i++) {
		Vec4 tmp = manVec4.createFromVec3(NULL, &mesh->norms[i], 0);
		tmp = manMat4.postMulVec4(matrix, &tmp);

		mesh->norms[i].x = tmp.x;
		mesh->norms[i].y = tmp.y;
		mesh->norms[i].z = tmp.z;
	}
}

static void deleteSimpleMesh(ColliderSimpleMesh* mesh) {
	free(mesh->norms);
	free(mesh->verts);
}

static void deleteComplexMesh(ColliderComplexMesh* mesh) {
	for(int i = 0; i < mesh->count; i++)
		deleteSimpleMesh(&mesh->meshes[i]);

	free(mesh->meshes);
}

static void deleteColliderSphere(ColliderSphere* mesh) {
	//Do nothing.
}


const CollisionMeshManager manColMesh = {newSimpleMesh, newComplexMesh, newColliderSphere, makeTransformationMatrix, transformSphere, copyMesh, transformSimpleMesh, deleteSimpleMesh, deleteComplexMesh, deleteColliderSphere};
