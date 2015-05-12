#ifndef MATH_VEC3_H
#define MATH_VEC3_H

#include <stdlib.h>

#include "Precision.h"

typedef struct Vec3_s {
    scalar x, y, z;
} Vec3;

typedef struct Vec3Manager_s {
    /**
     *  Create a Vec3 from the given x, y and z components. The Vec3 is returned as the
     *  the return-value of the function. Optionaly, the first argument may contain a pointer
     *  to a Vec3 to fill up with the constructed data.
     *
     *  @param  vec     const pointer to Vec3 to fill up with data, or NULL.
     *  @param  x       scalar x-component.
     *  @param  y       scalar y-component.
     *  @param  z       scalar z-component.
     *  @return         Vec3 with the given components.
     */
    Vec3 (*create)(Vec3 *const vec, scalar x, scalar y, scalar z);

    /**
     *  Create a Vec3 from the given Vec3. The Vec3 is returned as the
     *  the return-value of the function. Optionaly, the first argument may contain a pointer
     *  to a Vec3 to fill up with the constructed data.
     *
     *  @param  vec     const pointer to Vec3 to fill up with data, or NULL.
     *  @param  vec3    const pointer to const Vec3 to clone.
     *  @param          Vec3 clone.
     */
    Vec3 (*createFromVec3)(Vec3 *const vec, const Vec3 *const vec3);

    /**
     *  Return a Vec3 equal to v1 + v2 (component-wise addition).
     *
     *  @param  v1  const pointer to const Vec3 to sum.
     *  @param  v2  const pointer to const Vec3 to sum.
     *  @return     Vec3 equal to the component-wise operation v1 + v2.       
     */
    Vec3 (*sum)(const Vec3 *const v1, const Vec3 *const v2);

    /**
     *  Return a Vec3 equal to v1 - v2 (component-wise subtraction).
     *
     *  @param  v1  const pointer to const Vec3 to subtract from.
     *  @param  v2  const pointer to const Vec3 to subtract.
     *  @return     Vec3 equal to the component-wise operation v1 - v2.          
     */
    Vec3 (*sub)(const Vec3 *const v1, const Vec3 *const v2);

    /**
     *  Return a Vec3 where each of the components of the
     *  given Vec3 have been scaled by a constant factor.
     *
     *  @param vec      const pointer to const Vec3 to scale.
     *  @param factor   Scalar quantity to scale by.
     *  @return         Vec3 scaled by given factor.          
     */
    Vec3 (*postMulScalar)(const Vec3 *const vec, scalar factor);

    /**
     *  Return a Vec3 where each of the components of the
     *  given Vec3 have been scaled by a constant factor.
     *
     *  @param factor   Scalar quantity to scale by.
     *  @param vec      const pointer to const Vec3 to scale.
     *  @return         Vec3 scaled by given factor.          
     */
    Vec3 (*preMulScalar)(scalar factor, const Vec3 *const vec);

    /**
     *  Return a Vec3 with the same magnitude as the given Vec3
     *  but opposite direction.
     *
     *  @param  vec     const pointer to const Vec3 to invert.
     *  @return         Vec3 of equal magnitude but opposite direction to given Vec3.
     */
    Vec3 (*invert)(const Vec3 *const vec);

    /**
     *  Return the dot product of two Vec3s.
     *  
     *  @param  vec     const pointer to const Vec3, vector one.
     *  @param  vec     const pointer to const Vec3, vector two.
     *  @return         scalar representing dot product of two vectors.
     */
    scalar (*dot)(const Vec3 *const v1, const Vec3 *const v2);

    /**
     *  Returns a Vec3, the cross product of the two given Vec3s.
     *  (The Vec3 at right angles to the two other Vec3s)
     *
     *  @param  v1      const pointer to const Vec3, vector one.
     *  @param  vec     const pointer to const Vec3, vector two.
     *  @return         Vec3, cross product of two given Vec3s.
     */
    Vec3 (*cross)(const Vec3 *const v1, const Vec3 *const v2);

    /**
     *  Returns a unit-length Vec3 in the direction of the
     *  given Vec3.
     *
     *  Normalizing a vector with a magnitude of 0 has undefined results.
     *
     *  @param  vec     const pointer to const Vec3 to normalize.
     *  @return         Vec3 normalized.
     */
    Vec3 (*normalize)(const Vec3 *const vec);

    /**
     *  Returns the magnitude of the given Vec3.
     *
     *  @param vec  const pointer to const Vec3, given vector.
     *  @return     scalar representing the magnitude of the Vec3.
     */
    scalar (*magnitude)(const struct Vec3_s *const);
} Vec3Manager;

// Vec3     createVec3(scalar x, scalar y, scalar z);
// Vec3     createVec3Vec3(const Vec3 *const vec);

// Vec3     sumVec3Vec3(const Vec3 *const v1, const Vec3 *const v2);
// Vec3     subVec3Vec3(const Vec3 *const v1, const Vec3 *const v2);
// Vec3     mulVec3Scalar(const Vec3 *const vec, scalar factor);
// Vec3     mulScalarVec3(scalar factor, const Vec3 *const vec);
// Vec3     invertVec3(const Vec3 *const vec);
// scalar   dotVec3(const Vec3 *const v1, const Vec3 *const v2);
// Vec3     crossVec3(const Vec3 *const v1, const Vec3 *const v2);
// Vec3     normalizeVec3(const Vec3 *const vec);

extern const Vec3Manager manVec3;

#endif
