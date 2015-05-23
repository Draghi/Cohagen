#ifndef COH_COLLISIONRESPONSE_H
#define COH_COLLISIONRESPONSE_H

#include "math/Vec3.h"

void momentumCollisionResponse(Vec3* out1, Vec3* out2, Vec3 vel1, Vec3 vel2, scalar mass1, scalar mass2);
#endif
