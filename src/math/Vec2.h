#ifndef MATH_VEC2_H
#define MATH_VEC2_H

#include "Precision.h"

typedef struct Vec2_s {
    /**
     *  Return the magnitude of the Vec2.
     *
     * @param   self    const pointer to const Vec2 this function is operating on.
     * @return          scalar, the magnitude of the Vec2 given.
     */
    scalar (*magnitude)(const struct Vec2_s *const);

    scalar x, y;
} Vec2;

Vec2    createVec2(scalar x, scalar y);
Vec2    createVec2Vec2(const Vec2 *const vec);

Vec2    sumVec2Vec2(const Vec2 *const v1, const Vec2 *const v2);
Vec2    subVec2Vec2(const Vec2 *const v1, const Vec2 *const v2);
Vec2    mulVec2Scalar(const Vec2 *const vec, scalar factor);
Vec2    mulScalarVec2(scalar factor, const Vec2 *const vec);
Vec2    normalizeVec2(const Vec2 *const vec);
Vec2    invertVec2(const Vec2 *const vec);
scalar  dotVec2(const Vec2 *const v1, const Vec2 *const v2);

#endif