#include <math.h>

#include "Vec4.h"

static scalar magnitudeVec4(const Vec4 *const vec);

/**
 *  Returns a Vec4 composed of the given components.
 *
 *  @param  x   scalar, x-component.
 *  @param  y   scalar, y-component.
 *  @param  z   scalar, z-component.
 *  @param  w   scalar, w-component.
 *  @return     Vec4 composed of the given components.
 */
Vec4 createVec4(scalar x, scalar y, scalar z, scalar w) {
    Vec4 result;

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;

    //result.magnitude = magnitudeVec4;

    return (result);
}

/**
 *  Returns a Vec4 composed of the given Vec3 plus a given w-component.
 *
 *  @param  vec3    const pointer to const Vec3, x, y, z components to use.
 *  @param  w       scalar, w component to use.
 *  @return         Vec4 composed of given Vec3 x, y and z components
 *                  plus given w component.
 */
Vec4 createVec4Vec3(const Vec3 *const vec3, scalar w) {
    return (
        createVec4(
            vec3->x,
            vec3->y,
            vec3->z,
            w
        )
    );
}

/**
 *  Returns a Vec4, a copy of the given Vec4.
 *
 *  @param  vec     const pointer to const Vec4 to clone.
 *  @return         Vec4, clone
 */
Vec4 createVec4Vec4(const Vec4 *const vec) {
    return (
        createVec4(
            vec->x,
            vec->y,
            vec->z,
            vec->w
        )
    );
}

/**
 *  Returns a Vec4, the result of the component-wise sum operation
 *  between the given vectors. (ie. v1 + v2).
 *
 *  @param  v1  const pointer to const Vec4, operand 1.
 *  @param  v2  const pointer to const Vec4, operand 2.
 *  @return     Vec4, component-wise sum of given Vec4s.
 */
Vec4 sumVec4Vec4(const Vec4 *const v1, const Vec4 *const v2) {
    return (
        createVec4(
            v1->x + v2->x,
            v1->y + v2->y,
            v1->z + v2->z,
            v1->w + v2->w
        )
    );
}

/**
 *  Returns a Vec4, the result of the component-wise subtraction operation
 *  between the two given vectors. Where v1 is the operand to subtract from
 *  and v2 is the operand to subtract. (ie. v1 - v2).
 *
 *  @param  v1  const pointer to const Vec4, operand to subtract from.
 *  @param  v2  const pointer to const Vec4, operand to subtract.
 *  @return     Vec4, the result of v1 - v2.
 */
Vec4 subVec4Vec4(const Vec4 *const v1, const Vec4 *const v2) {
    return (
        createVec4(
            v1->x - v2->x,
            v1->y - v2->y,
            v1->z - v2->z,
            v1->w - v2->w
        )
    );
}

/**
 *  Returns a Vec4, the result of the given Vec4 with each
 *  component scaled by the given factor.
 *
 *  @param  vec     const pointer to const Vec4, vector to scale.
 *  @param  factor  scalar, factor to scale by.
 *  @return         Vec4, scaled version of given Vec4.   
 */
Vec4 mulVec4Scalar(const Vec4 *const vec, scalar factor) {
    return (
        createVec4(
            vec->x * factor,
            vec->y * factor,
            vec->z * factor,
            vec->w * factor
        )
    );
}

/**
 *  Returns a Vec4, the result of the given Vec4 with each
 *  component scaled by the given factor.
 *
 *  @param  vec     const pointer to const Vec4, vector to scale.
 *  @param  factor  scalar, factor to scale by.
 *  @return         Vec4, scaled version of given Vec4.   
 */
Vec4 mulScalarVec4(scalar factor, const Vec4 *const vec) {
    return (
        createVec4(
            vec->x * factor,
            vec->y * factor,
            vec->z * factor,
            vec->w * factor
        )
    );
}

/**
 *  Returns a Vec4, inverted version of the given Vec4.
 *
 *  @param  vec     const pointer to const Vec4 to invert.
 *  @return         Vec4, inverted version of given Vec4.
 */
Vec4 invertVec4(const Vec4 *const vec) {
    return (
        createVec4(
            -vec->x,
            -vec->y,
            -vec->z,
            -vec->w
        )
    );
}

/**
 *  Returns the dot product of two given Vec4s.
 *
 *  @param  v1  const pointer to const Vec4, operand 1.
 *  @param  v2  const pointer to const Vec4, operand 2.
 *  @return     scalar, dot product of two given vectors.
 */
scalar dotVec4(const Vec4 *const v1, const Vec4 *const v2) {
    return (
        v1->x * v2->x +
        v1->y * v2->y +
        v1->z * v2->z +
        v1->w * v2->w
    );
}

/**
 *  Returns a normalized version of the given Vec4.
 * 
 *  @param  vec     const pointer to const Vec4 to normalize.
 *  @return         Vec4, normalized version of given Vec4.
 */
Vec4 normalizeVec4(const Vec4 *const vec) {
    //scalar magnitude = vec->magnitude((const struct Vec4_s *const) vec);
    scalar magnitude = 1;
    
    return (
        createVec4(
            vec->x / magnitude,
            vec->y / magnitude,
            vec->z / magnitude,
            vec->w / magnitude
        )
    );
}

/**
 *  Returns the magnitude of the given Vec4.
 *
 *  @param  vec     const pointer to const Vec4 to find the magnitude of.
 *  @return         scalar, the magnitude of the Vec4 given.
 */
static scalar magnitudeVec4(const Vec4 *const vec) {
    return (sqrt(pow(vec->x, 2) + pow(vec->y, 2) + pow(vec->z, 2) + pow(vec->w, 2)));
}
