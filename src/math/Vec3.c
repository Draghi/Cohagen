#include <math.h>
#include "Vec3.h"

static scalar magnitudeVec3(const Vec3 *const self);

/**
 *  Return a Vec3 with the given x, y and z components.
 *
 *  @param  x   scalar x-component.
 *  @param  y   scalar y-component.
 *  @param  z   scalar z-component.
 *  @return     Vec3 with the given components.
 */
Vec3 createVec3(scalar x, scalar y, scalar z) {
    Vec3 result;

    result.x = x;
    result.y = y;
    result.z = z;

    result.magnitude = magnitudeVec3;

    return result;
}

/**
 *  Return a Vec3 which is a clone of the given Vec3
 *  (a copy constructor).
 *
 *  @param vec  const pointer to const Vec3 to clone.
 *  @param      Vec3 clone.
 */
Vec3 createVec3Vec3(const Vec3 *const vec) {
    return (
        createVec3(
            vec->x,
            vec->y,
            vec->z
        )
    );
}


/**
 *  Return a Vec3 equal to v1 + v2 (component-wise addition).
 *
 *  @param  v1  const pointer to const Vec3 to sum.
 *  @param  v2  const pointer to const Vec3 to sum.
 *  @return     Vec3 equal to the component-wise operation v1 + v2.       
 */
Vec3 sumVec3Vec3(const Vec3 *const v1, const Vec3 *const v2) {
    return (
        createVec3(
            v1->x + v2->x,
            v1->y + v2->y,
            v1->z + v2->z
        )
    );
}

/**
 *  Return a Vec3 equal to v1 - v2 (component-wise subtraction).
 *
 *  @param  v1  const pointer to const Vec3 to subtract from.
 *  @param  v2  const pointer to const Vec3 to subtract.
 *  @return     Vec3 equal to the component-wise operation v1 - v2.          
 */
Vec3 subVec3Vec3(const Vec3 *const v1, const Vec3 *const v2) {
   return (
        createVec3(
            v1->x - v2->x,
            v1->y - v2->y,
            v1->z - v2->z
        )
    );
}

/**
 *  Return a Vec3 where each of the components of the
 *  given Vec3 have been scaled by a constant factor.
 *
 *  @param vec      const pointer to const Vec3 to scale.
 *  @param factor   Scalar quantity to scale by.
 *  @return         Vec3 scaled by given factor.          
 */
Vec3 mulVec3Scalar(const Vec3 *const vec, scalar factor) {
    return (
        createVec3(
            factor * vec->x,
            factor * vec->y,
            factor * vec->z
        )
    );
}

/**
 *  Return a Vec3 where each of the components of the
 *  given Vec3 have been scaled by a constant factor.
 *
 *  @param factor   Scalar quantity to scale by.
 *  @param vec      const pointer to const Vec3 to scale.
 *  @return         Vec3 scaled by given factor.          
 */
Vec3 mulScalarVec3(scalar factor, const Vec3 *const vec) {
    return (
        createVec3(
            factor * vec->x,
            factor * vec->y,
            factor * vec->z
        )
    );
}

/**
 *  Return a Vec3 with the same magnitude as the given Vec3
 *  but opposite direction.
 *
 *  @param  vec     const pointer to const Vec3 to invert.
 *  @return         Vec3 of equal magnitude but opposite direction to given Vec3.
 */
Vec3 invertVec3(const Vec3 *const vec) {
    return (
        createVec3(
            -vec->x,
            -vec->y,
            -vec->z
        )
    );
}

/**
 *  Return the dot product of two Vec3s.
 *  
 *  @param  vec     const pointer to const Vec3, vector one.
 *  @param  vec     const pointer to const Vec3, vector two.
 *  @return         scalar representing dot product of two vectors.
 */
scalar dotVec3(const Vec3 *const v1, const Vec3 *const v2) {
    return (
        v1->x * v2->x +
        v1->y * v2->y +
        v1->z * v2->z
    );
}

/**
 *  Returns a Vec3, the cross product of the two given Vec3s.
 *  (The Vec3 at right angles to the two other Vec3s)
 *
 *  @param  v1      const pointer to const Vec3, vector one.
 *  @param  vec     const pointer to const Vec3, vector two.
 *  @return         Vec3, cross product of two given Vec3s.
 */
Vec3 crossVec3(const Vec3 *const v1, const Vec3 *const v2) {
    return (
        createVec3(
            v1->y * v2->z - v1->z * v2->y,
            v1->z * v2->x - v1->x * v2->z,
            v1->x * v2->y - v1->y * v2->x
        )
    );
}

/**
 *  Returns a unit-length Vec3 in the direction of the
 *  given Vec3.
 *
 *  Normalizing a vector with a magnitude of 0 has undefined results.
 *
 *  @param  vec     const pointer to const Vec3 to normalize.
 *  @return         Vec3 normalized.
 */
Vec3 normalizeVec3(const Vec3 *const vec) {
    scalar magnitude = vec->magnitude((const struct Vec3_s *const) vec);

    return (
        createVec3(
            vec->x / magnitude,
            vec->y / magnitude,
            vec->z / magnitude
        )
    );
}

/**
 *  Returns the magnitude of the given Vec3.
 *
 *  @param vec  const pointer to const Vec3, given vector.
 *  @return     scalar representing the magnitude of the Vec3.
 */
static scalar magnitudeVec3(const Vec3 *const self) {
    return (sqrt(pow(self->x, 2) + pow(self->y, 2) + pow(self->z, 2)));
}