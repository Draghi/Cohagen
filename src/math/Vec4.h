#ifndef MATH_VEC4_H
#define MATH_VEC4_H

#include "Precision.h"
#include "Vec3.h"

typedef struct Vec4_s {
    /**
     *  Returns the magnitude of the given Vec4.
     *
     *  @param vec  const pointer to const Vec4, given vector.
     *  @return     scalar representing the magnitude of the Vec4.
     */
    scalar (*magnitude)(const struct Vec4_s *const);

    scalar x, y, z, w;
} Vec4;

Vec4    createVec4(scalar x, scalar y, scalar z, scalar w);
Vec4    createVec4Vec3(const Vec3 *const vec3, scalar w);
Vec4    createVec4Vec4(const Vec4 *const vec);

Vec4    sumVec4Vec4(const Vec4 *const v1, const Vec4 *const v2);
Vec4    subVec4Vec4(const Vec4 *const v1, const Vec4 *const v2);
Vec4    mulVec4Scalar(const Vec4 *const vec, scalar factor);
Vec4    mulScalarVec4(scalar factor, const Vec4 *const vec);
Vec4    invertVec4(const Vec4 *const vec);
scalar  dotVec4(const Vec4 *const v1, const Vec4 *const v2);
Vec4    normalizeVec4(const Vec4 *const vec);

#endif