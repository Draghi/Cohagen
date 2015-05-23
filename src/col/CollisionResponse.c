#include "CollisionResponse.h"

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
