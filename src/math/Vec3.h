#ifndef MATH_VEC3_H
#define MATH_VEC3_H

#include "Precision.h"

typedef struct Vec3_s {
    /**
     *  Returns the magnitude of the given Vec3.
     *
     *  @param vec  const pointer to const Vec3, given vector.
     *  @return     scalar representing the magnitude of the Vec3.
     */
    scalar (*magnitude)(const struct Vec3_s *const);

    scalar x, y, z;
} Vec3;

Vec3     createVec3(scalar x, scalar y, scalar z);
Vec3     cloneVec3(const Vec3 *const vec);

Vec3     sumVec3Vec3(const Vec3 *const v1, const Vec3 *const v2);
Vec3     subVec3Vec3(const Vec3 *const v1, const Vec3 *const v2);
Vec3     mulVec3Scalar(const Vec3 *const vec, scalar factor);
Vec3     mulScalarVec3(scalar factor, const Vec3 *const vec);
Vec3     invertVec3(const Vec3 *const vec);
scalar   dotVec3(const Vec3 *const v1, const Vec3 *const v2);
Vec3     crossVec3(const Vec3 *const v1, const Vec3 *const v2);
Vec3     normalizeVec3(const Vec3 *const vec);

#endif