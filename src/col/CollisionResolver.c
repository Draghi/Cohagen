#include "col/CollisionResolver.h"

static CollisionResolver* new() {
	CollisionResolver* collisionResolver = malloc(sizeof(CollisionResolver));
	collisionResolver->colliders = manDynamicArray.new(1, sizeof(PhysicsCollider*));
	collisionResolver->collisionRecords = NULL;
	collisionResolver->transformedColliders = NULL;
	return collisionResolver;
}

static void resetTransformMesh(TransformedCollider* tCol) {
	if (tCol->collider.nPhase.satMesh.verts != NULL) {
		free(tCol->collider.nPhase.maxPointForAxis);
		free(tCol->collider.nPhase.minPointForAxis);
		free(tCol->collider.nPhase.satMesh.norms);
		free(tCol->collider.nPhase.satMesh.verts);

		tCol->collider.nPhase.maxPointForAxis = NULL;
		tCol->collider.nPhase.minPointForAxis = NULL;
		tCol->collider.nPhase.satMesh.norms = NULL;
		tCol->collider.nPhase.satMesh.verts = NULL;
		tCol->collider.nPhase.satMesh.vCount = 0;
		tCol->collider.nPhase.satMesh.nCount = 0;
	}
}

static void delete(CollisionResolver* collisionResolver) {
	manDynamicArray.delete(collisionResolver->colliders);
	free(collisionResolver->colliders);

	for(int i = 0; i < collisionResolver->transformedColliders->size; i++) {
		TransformedCollider* col = (TransformedCollider*)manDynamicArray.get(collisionResolver->transformedColliders, i);
		resetTransformMesh(col);
	}
	manDynamicArray.delete(collisionResolver->transformedColliders);
	free(collisionResolver->transformedColliders);

	manDynamicArray.delete(collisionResolver->collisionRecords);
	free(collisionResolver->collisionRecords);
}

static void addCollider(CollisionResolver* collisionResolver, PhysicsCollider* collider) {
	if (collider!=NULL) {
		manDynamicArray.append(collisionResolver->colliders, &collider);
	}
}

static void reset(CollisionResolver* collisionResolver) {
	if (collisionResolver->transformedColliders!=NULL) {
		for(int i = 0; i < collisionResolver->colliders->size; i++) {
			TransformedCollider* tCol = ((TransformedCollider*)manDynamicArray.get(collisionResolver->transformedColliders, i));
			resetTransformMesh(tCol);
		}
		manDynamicArray.delete(collisionResolver->transformedColliders);
		free(collisionResolver->transformedColliders);
	}
	collisionResolver->transformedColliders = manDynamicArray.new(collisionResolver->colliders->size, sizeof(TransformedCollider));

	for(int i = 0; i < collisionResolver->colliders->size; i++) {
		PhysicsCollider* collider = *(PhysicsCollider**)manDynamicArray.get(collisionResolver->colliders, i);
		TransformedCollider tCollider;

		PhysicsCollider* transformedCollider = manPhysCollider.new(collider->position, collider->rotation, collider->scale, collider->velocity, collider->inverseMass);
		tCollider.collider = *transformedCollider;
		tCollider.collider.nPhase.maxPointForAxis = NULL;
		tCollider.collider.nPhase.minPointForAxis = NULL;
		tCollider.collider.nPhase.satMesh.norms = NULL;
		tCollider.collider.nPhase.satMesh.verts = NULL;
		tCollider.collider.nPhase.satMesh.nCount = 0;
		tCollider.collider.nPhase.satMesh.vCount = 0;
		tCollider.hasMeshTransformed = false;

		free(transformedCollider);

		manDynamicArray.append(collisionResolver->transformedColliders, &tCollider);
	}
}

static void prepare(CollisionResolver* collisionResolver) {
	if (collisionResolver->collisionRecords!=NULL) {
			manDynamicArray.delete(collisionResolver->collisionRecords);
			free(collisionResolver->collisionRecords);
	}
	collisionResolver->collisionRecords = manDynamicArray.new(1, sizeof(CollisionRecord));

	//Transform all broadphase colliders.
	for(int i = 0; i < collisionResolver->transformedColliders->size; i++) {
		TransformedCollider* tCol = ((TransformedCollider*)manDynamicArray.get(collisionResolver->transformedColliders, i));
		PhysicsCollider* col = *(PhysicsCollider**)manDynamicArray.get(collisionResolver->colliders, i);

		Mat4 transformationMatrix = manColMesh.makeTransformationMatrix(col->position, col->rotation, col->scale);
		tCol->collider.bPhase = col->bPhase;

		manColMesh.transformSphere(&tCol->collider.bPhase, &transformationMatrix, col->scale);
	}
}

static void transformMesh(TransformedCollider* dest, PhysicsCollider* orginal) {
	dest->collider.nPhase = manColMesh.copyMesh(&orginal->nPhase);
	Mat4 transformationMatrix = manColMesh.makeTransformationMatrix(orginal->position, orginal->rotation, orginal->scale);
	manColMesh.transformSimpleMesh(&dest->collider.nPhase, &transformationMatrix);
}

static bool check(CollisionResolver* collisionResolver) {
	bool flag = false;
	//Do collision checks
	for(int i = 0; i < collisionResolver->transformedColliders->size; i++) {
		TransformedCollider* collider1 = (TransformedCollider*)manDynamicArray.get(collisionResolver->transformedColliders, i);

		//Compare against remaining objects
		for(int j = i+1; j < collisionResolver->transformedColliders->size; j++) {
			TransformedCollider* collider2 = (TransformedCollider*)manDynamicArray.get(collisionResolver->transformedColliders, j);

			if (manColDetection.checkStaticBroadphase(&collider1->collider.bPhase, &collider2->collider.bPhase)) {
				//Transform mesh if it needs to be.
				if (!collider1->hasMeshTransformed) {
					PhysicsCollider* orginal = *(PhysicsCollider**)manDynamicArray.get(collisionResolver->colliders, i);
					resetTransformMesh(collider1);
					transformMesh(collider1, orginal);
					collider1->hasMeshTransformed = true;
				}

				//Transform mesh if it needs to be.
				if (!collider2->hasMeshTransformed) {
					PhysicsCollider* orginal = *(PhysicsCollider**)manDynamicArray.get(collisionResolver->colliders, j);
					resetTransformMesh(collider2);
					transformMesh(collider2, orginal);
					collider2->hasMeshTransformed = true;
				}

				CollisionResult result = manColDetection.checkStaticNarrowphase(&collider1->collider.nPhase, &collider2->collider.nPhase);
				if (result.isColliding) {
					CollisionRecord record;
					record.collider1 = i;
					record.collider2 = j;
					record.collisionInfo = result;
					manDynamicArray.append(collisionResolver->collisionRecords, &record);
					flag = true;
				}
			}

		}
	}

	return flag;
}

void momentumCollisionResponse(Vec3* out1, Vec3* out2, Vec3 vel1, Vec3 vel2, scalar mass1, scalar mass2) {
	scalar v1LeftM = (mass1 - mass2)/(mass1 + mass2);
	scalar v1RightM = (2*mass2)/(mass1 + mass2);
	Vec3 v1Left = manVec3.preMulScalar(v1LeftM, &vel1);
	Vec3 v1Right = manVec3.preMulScalar(v1RightM, &vel2);
	*out1 = manVec3.sum(&v1Left, &v1Right);

	scalar v2LeftM = (mass2 - mass1)/(mass1 + mass2);
	scalar v2RightM = (2*mass1)/(mass1 + mass2);
	Vec3 v2Left = manVec3.preMulScalar(v2LeftM, &vel2);
	Vec3 v2Right = manVec3.preMulScalar(v2RightM, &vel1);
	*out2 = manVec3.sum(&v2Left, &v2Right);
}

static void resolve(CollisionResolver* collisionResolver) {
	for(int i = 0; i < collisionResolver->collisionRecords->size; i++) {
		CollisionRecord* cr = (CollisionRecord*)manDynamicArray.get(collisionResolver->collisionRecords, i);
		TransformedCollider* collider1 = (TransformedCollider*)manDynamicArray.get(collisionResolver->transformedColliders, cr->collider1);
		TransformedCollider* collider2 = (TransformedCollider*)manDynamicArray.get(collisionResolver->transformedColliders, cr->collider2);

		Vec3 translation = manVec3.preMulScalar(cr->collisionInfo.distance/2, &cr->collisionInfo.axis);

		*collider1->collider.position = manVec3.sum(collider1->collider.position, &translation);
		translation = manVec3.invert(&translation);
		*collider2->collider.position = manVec3.sum(collider2->collider.position, &translation);

		collider1->hasMeshTransformed = false;
		collider2->hasMeshTransformed = false;

		momentumCollisionResponse(collider1->collider.velocity,  collider2->collider.velocity,
		                         *collider1->collider.velocity, *collider2->collider.velocity,
		                          1/(*collider1->collider.inverseMass), 1/(*collider2->collider.inverseMass));
	}
}

const CollisionResolverManager manColResolver = {new, addCollider, reset, prepare, check, resolve, delete};
