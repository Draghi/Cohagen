#include "CollisionMesh.h"

static const Vec3 xAxis = {1, 0, 0};
static const Vec3 yAxis = {0, 1, 0};
static const Vec3 zAxis = {0, 0, 1};

static ColliderSimpleMesh* newSimpleMesh(int vertCount, Vec3* verts, int normCount, Vec3* norms, int* minPoints, int* maxPoints) {
	ColliderSimpleMesh* newMesh = malloc(sizeof(ColliderSimpleMesh));

	newMesh->satMesh.vCount = vertCount;
	newMesh->satMesh.verts = verts;
	newMesh->satMesh.nCount = normCount;
	newMesh->satMesh.norms = norms;
	newMesh->minPointForAxis = minPoints;
	newMesh->maxPointForAxis = maxPoints;

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
	dest.satMesh.vCount = mesh->satMesh.vCount;
	dest.satMesh.verts = malloc(sizeof(Vec3)*dest.satMesh.vCount);
	for(int i = 0; i < dest.satMesh.vCount; i++) {
		dest.satMesh.verts[i] = mesh->satMesh.verts[i];
	}

	dest.satMesh.nCount = mesh->satMesh.nCount;
	dest.satMesh.norms = malloc(sizeof(Vec3)*dest.satMesh.nCount);
	for(int i = 0; i < dest.satMesh.nCount; i++) {
		dest.satMesh.norms[i] = mesh->satMesh.norms[i];
	}

	if (mesh->minPointForAxis != NULL) {
		dest.minPointForAxis = malloc(sizeof(Vec3)*dest.satMesh.nCount);
		for(int i = 0; i < dest.satMesh.nCount; i++) {
			dest.minPointForAxis[i] = mesh->minPointForAxis[i];
		}
	}

	if (mesh->maxPointForAxis != NULL) {
		dest.maxPointForAxis = malloc(sizeof(Vec3)*dest.satMesh.nCount);
		for(int i = 0; i < dest.satMesh.nCount; i++) {
			dest.maxPointForAxis[i] = mesh->maxPointForAxis[i];
		}
	}

	return dest;
}

static void transformSimpleMesh(ColliderSimpleMesh* mesh, Mat4* matrix) {
	for(int i = 0; i < mesh->satMesh.vCount; i++) {
		Vec4 tmp = manVec4.createFromVec3(NULL, &mesh->satMesh.verts[i], 1);
		tmp = manMat4.postMulVec4(matrix, &tmp);

		mesh->satMesh.verts[i].x = tmp.x;
		mesh->satMesh.verts[i].y = tmp.y;
		mesh->satMesh.verts[i].z = tmp.z;
	}

	for(int i = 0; i < mesh->satMesh.nCount; i++) {
		Vec4 tmp = manVec4.createFromVec3(NULL, &mesh->satMesh.norms[i], 0);
		tmp = manMat4.postMulVec4(matrix, &tmp);

		mesh->satMesh.norms[i].x = tmp.x;
		mesh->satMesh.norms[i].y = tmp.y;
		mesh->satMesh.norms[i].z = tmp.z;
	}
}

static void deleteSimpleMesh(ColliderSimpleMesh* mesh) {
	free(mesh->satMesh.norms);
	free(mesh->satMesh.verts);
	free(mesh->minPointForAxis);
	free(mesh->maxPointForAxis);
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
