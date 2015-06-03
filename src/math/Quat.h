#ifndef MATH_QUAT_H
#define MATH_QUAT_H

#include <stdlib.h>

#include "Precision.h"
#include "Vec3.h"
#include "Mat4.h"

/**
 *  Quaternion class.
 *
 *  Precision set in "Precision.h"
 */
typedef struct Quat_s {
    scalar x, y, z, w;
} Quat;

/**
 *  Class used to manage Quaternions.
 */
typedef struct QuatManager_s {
    /**
     *  Create a Quaternion from the given x, y and z components. The Quaternion is
     *  returned as the return-value of the function. Optionally, the first argument
     *  may contain a pointer to a Quaternion to fill up with the constructed data.
     *
     *  @param      quat    const pointer to Quat, Quaternion to set-up or NULL.
     *  @param      x       scalar, rotation axis x-component.
     *  @param      y       scalar, rotation axis y-component.
     *  @param      z       scalar, rotation axis z-component.
     *  @param      w       scalar, scalar component
     *  @return             Quat, constructed from given components.
     */
    Quat (*create)(Quat *const quat, scalar x, scalar y, scalar z, scalar w);

    /**
     *  Create a Quaternion from the given axis and scalar components. The Quaternion is
     *  returned as the return-value of the function. Optionally, the first argument
     *  may contain a pointer to a Quaternion to fill up with the constructed data.
     *
     *  @param     quat    const pointer to Quat, Quaternion to set-up or NULL.
     *  @param      axis    Vec3, rotation axis.
     *  @param      scalar  scalar, scalar component
     *  @return             Quat, constructed from given components.
     */
    Quat (*createFromAxisScalar)(Quat *const quat, const Vec3 *const axis, scalar scalar);
    
    /**
     *  Return a Quaternion with the same rotation axis as the given
     *  Quaternion but opposite rotation angle.
     *
     *  @param  quat    const pointer to const Quat to base calcuation off.
     *  @return         Quat with the same rotation axis as given Quaternion but
     *                  opposite rotation angle.
     */
    Quat (*invert)(const Quat *const quat);

    /**
     *  Multiplies q1 with q2 and returns the result. The result is
     *  the summation of a rotation q2 followed by rotation q1 (ie. q1 * q2).
     * 
     *  @param  q1  const pointer to const Quat, operand 1.
     *  @param  q2  const pointer to const Quat, operand 2.
     *  @return     Quat, the quaternion representing the result of a
     *              rotation q2 followed by rotation q1 (ie. q1 * q2).
     */
    Quat (*mul)(const Quat *const q1, const Quat *const q2);

    /**
     *  Returns a normalized copy of the given Quaternion. The result is
     *  a unit Quaternion or "versor".
     *
     *  @param quat const pointer to const Quat, quaternion to normalize.
     *  @return     Quat, normalized copy of given Quat (versor).
     */
    Quat (*normalize)(const Quat *const quat);

    /**
     *  Calculates the dot product of two given quaternions.
     *
     *  @param  q1  Quat, operand 1.
     *  @param  q2  Quat, operand 2.
     *  @return     scalar, dot product of two given quaternions.
     */
    scalar (*dot)(const Quat *const q1, const Quat *const q2);

    /**
     *  Converts the given quaternion to a matrix, useful for rotations.
     *
     *  @param  quat    Quat, quaternion to cast to Mat4.
     *  @return         Mat4, transformation matrix representing the same rotation
     *                  as the given quaternion.
     */
    Mat4 (*castMat4)(const Quat *const quat);

    /** 
     *  Offset given Quaternion around given axis by given number
     *  of radians.
     *  
     *  @param  quat        Quat to operate on.
     *  @param  axis        Vec3, axis to rotate around.
     *  @param  angleRad    float, angle in radians to rotate.
     */
    void (*offsetAxis)(Quat *const quat, const Vec3 *const axis, float angleRad);

    /** 
     *  Offset given Quaternion around given axis by given number
     *  of radians.
     *  
     *  @param  quat        Quat to operate on.
     *  @param  x           scalar, x-component of rotation axis.
     *  @param  y           scalar, y-component of rotation axis.
     *  @param  z           scalar, z-component of rotation axis.
     *  @param  angleRad    float, angle in radians to rotate.
     */
    void    (*offsetAxisXYZ)(Quat *const quat, scalar x, scalar y, scalar z, float angleRad);

    /**
     *  Calculates the magnitude of the given Quaternion.
     *
     *  @param quat     Quat to calculate magnitude of.
     *  @return         scalar, magnitude of given Quaternion.
     */
    scalar  (*magnitude)(const Quat *const quat);
} QuatManager;

// Quat    createQuat(scalar x, scalar y, scalar z, scalar w);
// Quat    createQuatAxisScalar(const Vec3 *const axis, scalar scalar);
// Quat    invertQuat(const Quat *const quat);
// Quat    mulQuatQuat(const Quat *const q1, const Quat *const q2);
// Quat    normalizeQuat(const Quat *const quat);
// scalar  dotQuat(const Quat *const q1, const Quat *const q2);
// Mat4    castQuatMat4(const Quat *const quat);

extern const QuatManager manQuat;

#endif
