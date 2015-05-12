#ifndef MATH_VEC2_H
#define MATH_VEC2_H

#include <stdlib.h>

#include "Precision.h"

typedef struct Vec2_s {
    scalar x, y;
} Vec2;

typedef struct Vec2Manager_s {
    /**
     *  Create a Vec2 from the given x and y components. The Vec2 is returned as the
     *  the return-value of the function. Optionaly, the first argument may contain a pointer
     *  to a Vec2 to fill up with the constructed data.
     *
     *  @param  vec     const pointer to Vec2 to fill up with data, or NULL.
     *  @param  x       scalar, value for x.
     *  @param  y       scalar, value for y.
     *  @return         Vec2 with given x and y components.           
     */
    Vec2 (*create)(Vec2 *const vec, scalar x, scalar y);

    /**
     *  Create a Vec2 from the given Vec2. The Vec2 is returned as the return-value
     *  of the function. Optionally, the first argument may contain a pointer to a
     *  Vec2 to fill up with the constructed data.
     *
     *  @param  vec     const pointer to Vec2 to fill up with data, or NULL.
     *  @param  vec2    const pointer to const Vec2 to clone.
     *  @return         Vec2, clone.
     */
    Vec2 (*createFromVec2)(Vec2 *const vec, const Vec2 *const vec2);

    /**
     *  Returns the component-wise sum of two Vec2s. (ie. v1 + v2)
     *
     *  @param v1   const pointer to const Vec2, first operand.
     *  @param v2   const pointer to const Vec2, second operand.
     *  @return     Vec2, component-wise sum of the two Vec2s given.
     */
    Vec2 (*sum)(const Vec2 *const v1, const Vec2 *const v2);

    /**
     *  Returns the component-wise subtraction of v2 from v1. (ie. v1 - v2).
     *
     *  @param v1   const pointer to const Vec2 to subtract from.
     *  @param v2   const pointer to const Vec2 to subtract.
     *  @return     Vec2, result of the component-wise subtraction of v2 from v1.
     */
    Vec2 (*sub)(const Vec2 *const v1, const Vec2 *const v2);

    /**
     *  Returns a Vec2 in the same direction as the given Vec2
     *  but with the magnitude scaled by the given factor.
     *
     *  @param vec      const pointer to const Vec2 to scale.
     *  @param factor   scalar, scaling factor.
     *  @return         Vec2, given vec scaled by a given factor.
     */
    Vec2 (*postMulScalar)(const Vec2 *const vec, scalar factor);

    /**
     *  Returns a Vec2 in the same direction as the given Vec2
     *  but with the magnitude scaled by the given factor.
     *
     *  @param factor   scalar, scaling factor.
     *  @param vec      const pointer to const Vec2 to scale.
     *  @return         Vec2, given vec scaled by a given factor.
     */
    Vec2 (*preMulScalar)(scalar factor, const Vec2 *const vec);

    /**
     *  Returns the normalized version of the given Vec2.
     *
     *  @param  vec     const pointer to the Vec2 this function is operating on.
     *  @return         Vec2, normalized version of given vec.
     */
    Vec2 (*normalize)(const Vec2 *const vec);

    /**
     *  Invert the direction of this Vec2 and return the inverse as the result.
     * 
     *  @param  vec         const pointer to the Vec2 this function is operating on.
     *  @return             Vec2 in opposite direction but of equal magnitude to the Vec2 given.
     */
    Vec2 (*invert)(const Vec2 *const vec);

    /**
     *  Returns the dot product of the two given vectors.
     *
     *  @param v1   const pointer to const Vec2, vector 1.
     *  @param v2   const pointer to const Vec2, vector 2.
     *  @return     scalar, dot product.
     */
    scalar (*dot)(const Vec2 *const v1, const Vec2 *const v2);

    /**
     *  Return the magnitude of the Vec2.
     *
     * @param   vec     const pointer to const Vec2 this function is operating on.
     * @return          scalar, the magnitude of the Vec2 given.
     */
    scalar (*magnitude)(const Vec2 *const vec);
} Vec2Manager;

// Vec2    createVec2(scalar x, scalar y);
// Vec2    createVec2Vec2(const Vec2 *const vec);

// Vec2    sumVec2Vec2(const Vec2 *const v1, const Vec2 *const v2);
// Vec2    subVec2Vec2(const Vec2 *const v1, const Vec2 *const v2);
// Vec2    mulVec2Scalar(const Vec2 *const vec, scalar factor);
// Vec2    mulScalarVec2(scalar factor, const Vec2 *const vec);
// Vec2    normalizeVec2(const Vec2 *const vec);
// Vec2    invertVec2(const Vec2 *const vec);
// scalar  dotVec2(const Vec2 *const v1, const Vec2 *const v2);

extern const Vec2Manager manVec2;

#endif
