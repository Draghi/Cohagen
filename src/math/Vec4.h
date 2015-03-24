#ifndef MATH_VEC4_H
#define MATH_VEC4_H

#include "Precision.h"
#include "Vec3.h"

typedef struct Vec4_s {
    scalar x, y, z, w;
} Vec4;

typedef struct Vec4Manager_s {
    /**
     *  Create a Vec4 from the given x, y, z and w components. The Vec4 is
     *  returned as the return-value of the function. Optionally, the first argument
     *  may contain a pointer to a Vec4 to fill up with the constructed data.
     *
     *  @param  vec     const pointer to Vec4, Vec4 to set-up or NULL.
     *  @param  x       scalar, x-component.
     *  @param  y       scalar, y-component.
     *  @param  z       scalar, z-component.
     *  @param  w       scalar, w-component.
     *  @return         Vec4 composed of the given components.
     */
    Vec4 (*create)(Vec4 *const vec, scalar x, scalar y, scalar z, scalar w);

    /**
     *  Create a Vec4 from the given Vec3 plus a w component. The Vec4 is
     *  returned as the return-value of the function. Optionally, the first argument
     *  may contain a pointer to a Vec4 to fill up with the constructed data.
     *
     *  @param  vec     const pointer to Vec4, Vec4 to set-up or NULL.
     *  @param  vec3    const pointer to const Vec3, x, y, z components to use.
     *  @param  w       scalar, w component to use.
     *  @return         Vec4 composed of given Vec3 x, y and z components
     *                  plus given w component.
     */
    Vec4 (*createFromVec3)(Vec4 *const vec, const Vec3 *const vec3, scalar w);

    /**
     *  Create a Vec4 from the given Vec4. The Vec4 is
     *  returned as the return-value of the function. Optionally, the first argument
     *  may contain a pointer to a Vec4 to fill up with the constructed data.
     *
     *  @param  vec     const pointer to Vec4, Vec4 to set-up or NULL.
     *  @param  vec4    const pointer to const Vec4 to clone.
     *  @return         Vec4, clone
     */
    Vec4 (*createFromVec4)(Vec4 *const vec, const Vec4 *const vec4);

    /**
     *  Returns a Vec4, the result of the component-wise sum operation
     *  between the given vectors. (ie. v1 + v2).
     *
     *  @param  v1  const pointer to const Vec4, operand 1.
     *  @param  v2  const pointer to const Vec4, operand 2.
     *  @return     Vec4, component-wise sum of given Vec4s.
     */
    Vec4 (*sum)(const Vec4 *const v1, const Vec4 *const v2);

    /**
     *  Returns a Vec4, the result of the component-wise subtraction operation
     *  between the two given vectors. Where v1 is the operand to subtract from
     *  and v2 is the operand to subtract. (ie. v1 - v2).
     *
     *  @param  v1  const pointer to const Vec4, operand to subtract from.
     *  @param  v2  const pointer to const Vec4, operand to subtract.
     *  @return     Vec4, the result of v1 - v2.
     */
    Vec4 (*sub)(const Vec4 *const v1, const Vec4 *const v2);

    /**
     *  Returns a Vec4, the result of the given Vec4 with each
     *  component scaled by the given factor.
     *
     *  @param  vec     const pointer to const Vec4, vector to scale.
     *  @param  factor  scalar, factor to scale by.
     *  @return         Vec4, scaled version of given Vec4.   
     */
    Vec4 (*postMulScalar)(const Vec4 *const vec, scalar factor);

    /**
     *  Returns a Vec4, the result of the given Vec4 with each
     *  component scaled by the given factor.
     *
     *  @param  vec     const pointer to const Vec4, vector to scale.
     *  @param  factor  scalar, factor to scale by.
     *  @return         Vec4, scaled version of given Vec4.   
     */
    Vec4 (*preMulScalar)(scalar factor, const Vec4 *const vec);

    /**
     *  Returns a Vec4, inverted version of the given Vec4.
     *
     *  @param  vec     const pointer to const Vec4 to invert.
     *  @return         Vec4, inverted version of given Vec4.
     */
    Vec4 (*invert)(const Vec4 *const vec);

    /**
     *  Returns the dot product of two given Vec4s.
     *
     *  @param  v1  const pointer to const Vec4, operand 1.
     *  @param  v2  const pointer to const Vec4, operand 2.
     *  @return     scalar, dot product of two given vectors.
     */
    scalar (*dot)(const Vec4 *const v1, const Vec4 *const v2);

    /**
     *  Returns a normalized version of the given Vec4.
     * 
     *  @param  vec     const pointer to const Vec4 to normalize.
     *  @return         Vec4, normalized version of given Vec4.
     */
    Vec4 (*normalize)(const Vec4 *const vec);

    /**
     *  Returns the magnitude of the given Vec4.
     *
     *  @param vec  const pointer to const Vec4, given vector.
     *  @return     scalar representing the magnitude of the Vec4.
     */
    scalar (*magnitude)(const struct Vec4_s *const);
} Vec4Manager;

// Vec4    createVec4(scalar x, scalar y, scalar z, scalar w);
// Vec4    createVec4Vec3(const Vec3 *const vec3, scalar w);
// Vec4    createVec4Vec4(const Vec4 *const vec);

// Vec4    sumVec4Vec4(const Vec4 *const v1, const Vec4 *const v2);
// Vec4    subVec4Vec4(const Vec4 *const v1, const Vec4 *const v2);
// Vec4    mulVec4Scalar(const Vec4 *const vec, scalar factor);
// Vec4    mulScalarVec4(scalar factor, const Vec4 *const vec);
// Vec4    invertVec4(const Vec4 *const vec);
// scalar  dotVec4(const Vec4 *const v1, const Vec4 *const v2);
// Vec4    normalizeVec4(const Vec4 *const vec);

extern const Vec4Manager manVec4;

#endif