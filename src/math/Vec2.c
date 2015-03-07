#include <math.h>

#include "Vec2.h"

static scalar magnitudeVec2(const Vec2 *const self);

/**
 *  Returns a Vec2 composed of the given x and y values.
 *
 *  @param  x   scalar, value for x.
 *  @param  y   scalar, value for y.
 *  @return     Vec2 with given x and y components.           
 */
Vec2 createVec2(scalar x, scalar y) {
    Vec2 result;

    result.x = x;
    result.y = y;

    result.magnitude = magnitudeVec2;

    return result;
}

/**
 *  Returns a copy of the given Vec2.
 *
 *  @param  vec     const pointer to const Vec2 to clone.
 *  @return         Vec2, clone.
 */
Vec2 cloneVec2(const Vec2 *const vec) {
    return (
        createVec2(
            vec->x,
            vec->y
        )
    );
}

/**
 *  Returns magnitude of given Vec2.
 *
 *  @param  self    const pointer to const Vec2 to find magnitude of.
 *  @return         scalar, magnitude of given Vec2
 */
static scalar magnitudeVec2(const Vec2 *const self) {
    return (sqrt(pow(self->x, 2) + pow(self->y, 2)));
}

/**
 *  Returns the component-wise sum of two Vec2s. (ie. v1 + v2)
 *
 *  @param v1   const pointer to const Vec2, first operand.
 *  @param v2   const pointer to const Vec2, second operand.
 *  @return     Vec2, component-wise sum of the two Vec2s given.
 */
Vec2 sumVec2Vec2(const Vec2 *const v1, const Vec2 *const v2) {
    return (
        createVec2(
            v1->x + v2->x,
            v1->y + v2->y
        )
    );
}

/**
 *  Returns the component-wise subtraction of v2 from v1. (ie. v1 - v2).
 *
 *  @param v1   const pointer to const Vec2 to subtract from.
 *  @param v2   const pointer to const Vec2 to subtract.
 *  @return     Vec2, result of the component-wise subtraction of v2 from v1.
 */
Vec2 subVec2Vec2(const Vec2 *const v1, const Vec2 *const v2) {
    return (
        createVec2(
            v1->x - v2->x,
            v1->y - v2->y
        )
    );
}

/**
 *  Returns a Vec2 in the same direction as the given Vec2
 *  but with the magnitude scaled by the given factor.
 *
 *  @param vec      const pointer to const Vec2 to scale.
 *  @param factor   scalar, scaling factor.
 *  @return         Vec2, given vec scaled by a given factor.
 */
Vec2 mulVec2Scalar(const Vec2 *const vec, scalar factor) {
    return (
        createVec2(
            vec->x * factor,
            vec->y * factor
        )
    );
}

/**
 *  Returns a Vec2 in the same direction as the given Vec2
 *  but with the magnitude scaled by the given factor.
 *
 *  @param factor   scalar, scaling factor.
 *  @param vec      const pointer to const Vec2 to scale.
 *  @return         Vec2, given vec scaled by a given factor.
 */
Vec2 mulScalarVec2(scalar factor, const Vec2 *const vec) {
    return (
        createVec2(
            vec->x * factor,
            vec->y * factor
        )
    );    
}

/**
 *  Returns the normalized version of the given Vec2.
 *
 *  @param  vec     const pointer to the Vec2 this function is operating on.
 *  @return         Vec2, normalized version of given vec.
 */
Vec2 normalizeVec2(const Vec2 *const vec) {
    scalar magnitude = vec->magnitude((const struct Vec2_s *const) &vec);

    return (
        createVec2(
            vec->x / magnitude,
            vec->y / magnitude
        )
    );
}

/**
 *  Invert the direction of this Vec2 and return the inverse as the result.
 * 
 *  @param  vec         const pointer to the Vec2 this function is operating on.
 *  @return             Vec2 in opposite direction but of equal magnitude to the Vec2 given.
 */
Vec2 invertVec2(const Vec2 *const vec) {
    return (
        createVec2(
            -vec->x,
            -vec->y
        )
    );   
}

/**
 *  Returns the dot product of the two given vectors.
 *
 *  @param v1   const pointer to const Vec2, vector 1.
 *  @param v2   const pointer to const Vec2, vector 2.
 *  @return     scalar, dot product.
 */
scalar dotVec2(const Vec2 *const v1, const Vec2 *const v2) {
   return (
        v1->x * v2->x +
        v1->y * v2->y
    ); 
}