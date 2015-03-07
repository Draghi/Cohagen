#ifndef MATH_QUAT_H
#define MATH_QUAT_H

#include "Precision.h"
#include "Vec3.h"
#include "Mat4.h"

typedef struct Quat_s {
    /** 
     *  Offset given Quaternion around given axis by given number
     *  of radians.
     *  
     *  @param  self    Quat to operate on.
     *  @param  axis    Vec3, axis to rotate around.
     *  @param  angle   float, angle in radians to rotate.
     */
    void    (*offsetAxis)(struct Quat_s *const, const Vec3 *const, float);

    /** 
     *  Offset given Quaternion around given axis by given number
     *  of radians.
     *  
     *  @param  self    Quat to operate on.
     *  @param  x       scalar, x-component of rotation axis.
     *  @param  y       scalar, y-component of rotation axis.
     *  @param  z       scalar, z-component of rotation axis.
     *  @param  angle   float, angle in radians to rotate.
     */
    void    (*offsetAxisXYZ)(struct Quat_s *const, scalar, scalar, scalar, float);

    /**
     *  Calculates the magnitude of the given Quaternion.
     *
     *  @param self     Quat to calculate magnitude of.
     *  @return         scalar, magnitude of given Quaternion.
     */
    scalar  (*magnitude)(const struct Quat_s *const);

    scalar x, y, z, w;
} Quat;

Quat    createQuat(scalar x, scalar y, scalar z, scalar w);
Quat    createQuatAxisScalar(const Vec3 *const axis, scalar scalar);
Quat    invertQuat(const Quat *const quat);
Quat    mulQuatQuat(const Quat *const q1, const Quat *const q2);
Quat    normalizeQuat(const Quat *const quat);
scalar  dotQuat(const Quat *const q1, const Quat *const q2);
Mat4    castQuatMat4(const Quat *const quat);

#endif