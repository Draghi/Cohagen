#include <math.h>
#include <stdio.h>

#include "Quat.h"

static void offsetAxis(struct Quat_s *const self, const Vec3 *const axis, float angle);
static void offsetAxisXYZ(struct Quat_s *const self, scalar x, scalar y, scalar z, float angle);
static scalar magnitudeQuat(const struct Quat_s *const self);

/**
 *  Returns a Quaternion constructed from the given x, y, z rotation axis
 *  and scalar w.
 *
 *  @param  x   scalar, rotation axis x-component.
 *  @param  y   scalar, rotation axis y-component.
 *  @param  z   scalar, rotation axis z-component.
 *  @param  w   scalar, scalar component
 *  @return     Quat, constructed from given components.
 */
Quat createQuat(scalar x, scalar y, scalar z, scalar w) {
    Quat quaternion;

    quaternion.x = x;
    quaternion.y = y;
    quaternion.z = z;
    quaternion.w = w;

    quaternion.offsetAxis = offsetAxis;
    quaternion.offsetAxisXYZ = offsetAxisXYZ;
    quaternion.magnitude = magnitudeQuat;

    return quaternion;
}

/**
 *  Returns a Quaternion constructed from the given x, y, z rotation axis
 *  and scalar w.
 *
 *  @param  axis    Vec3, rotation axis.
 *  @param  w       scalar, scalar component
 *  @return         Quat, constructed from given components.
 */
Quat createQuatAxisScalar(const Vec3 *const axis, scalar scalar) {
    return (
        createQuat(
            axis->x,
            axis->y,
            axis->z,
            scalar
        )
    );
}

/**
 *  Return a Quaternion with the same rotation axis as the given
 *  Quaternion but opposite rotation angle.
 *
 *  @param  quat    const pointer to const Quat to base calcuation off.
 *  @return         Quat with the same rotation axis as given Quaternion but
 *                  opposite rotation angle.
 */
Quat invertQuat(const Quat *const quat) {
    return (
        createQuat(
            quat->x,
            quat->y,
            quat->z,
            -quat->w
        )
    );
}

/**
 *  Multiplies q1 with q2 and returns the result. The result is
 *  the summation of a rotation q2 followed by rotation q1 (ie. q1 * q2).
 * 
 *  @param  q1  const pointer to const Quat, operand 1.
 *  @param  q2  const pointer to const Quat, operand 2.
 *  @return     Quat, the quaternion representing the result of a
 *              rotation q2 followed by rotation q1 (ie. q1 * q2).
 */
Quat mulQuatQuat(const Quat *const q1, const Quat *const q2) {
    return (
        createQuat(   
            q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y,
            q1->w * q2->y + q1->y * q2->w + q1->z * q2->x - q1->x * q2->z,
            q1->w * q2->z + q1->z * q2->w + q1->x * q2->y - q1->y * q2->x,
            q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z
        )
    );
}

/**
 *  Returns a normalized copy of the given Quaternion. The result is
 *  a unit Quaternion or "versor".
 *
 *  @param quat const pointer to const Quat, quaternion to normalize.
 *  @return     Quat, normalized copy of given Quat (versor).
 */
Quat normalizeQuat(const Quat *const quat) {
    scalar magnitude = quat->magnitude((const struct Quat_s *const) quat);

    return (
        createQuat(
            quat->x / magnitude,
            quat->y / magnitude,
            quat->z / magnitude,
            quat->w / magnitude
        )
    );
}

/**
 *  Calculates the dot product of two given quaternions.
 *
 *  @param  q1  Quat, operand 1.
 *  @param  q2  Quat, operand 2.
 *  @return     scalar, dot product of two given quaternions.
 */
scalar dotQuat(const Quat *const q1, const Quat *const q2) {
    return (
        q1->x * q2->x + 
        q1->y * q2->y +
        q1->z * q2->z +
        q1->w * q2->w
    );
}

/**
 *  Converts the given quaternion to a matrix, useful for rotations.
 *
 *  @param  quat    Quat, quaternion to cast to Mat4.
 *  @return         Mat4, transformation matrix representing the same rotation
 *                  as the given quaternion.
 */
Mat4 castQuatMat4(const Quat *const quat) {
    // Mat4f columns
    Vec4 col0 = createVec4(1 - 2*quat->y*quat->y - 2*quat->z*quat->z, 2*quat->x*quat->y + 2*quat->w*quat->z, 2*quat->x*quat->z - 2*quat->w*quat->y, 0.0f);
    Vec4 col1 = createVec4(2*quat->x*quat->y - 2*quat->w*quat->z, 1 - 2*quat->x*quat->x - 2*quat->z*quat->z, 2*quat->y*quat->z + 2*quat->w*quat->x, 0.0f);
    Vec4 col2 = createVec4(2*quat->x*quat->z + 2*quat->w*quat->y, 2*quat->y*quat->z - 2*quat->w*quat->x, 1 - 2*quat->x*quat->x - 2*quat->y*quat->y, 0.0f);
    Vec4 col3 = createVec4(0.0f, 0.0f, 0.0f, 1.0f);
        
    return ( createMat4Vec4(&col0, &col1, &col2, &col3) );
}

/** 
 *  Offset given Quaternion around given axis by given number
 *  of radians. Quaternion is not guaranteed to be normalized after this function
 *  is called.
 *  
 *  @param  self    Quat to operate on.
 *  @param  axis    Vec3, axis to rotate around.
 *  @param  angle   float, angle in radians to rotate.
 */
static void offsetAxis(struct Quat_s *const self, const Vec3 *const axis, float angle) {
    Vec3 axisN = normalizeVec3(axis);

    axisN = mulVec3Scalar(&axisN, sinf(angle / 2.0f));
    scalar scalar = cosf(angle / 2.0f);

    Quat offset = createQuatAxisScalar(&axisN, scalar);
    
    *self = mulQuatQuat(self, &offset);

    /**
     *  I've commented this line out because trying to normalize a (0, 0, 0, 0) quaternion will cause a divide by zero error.
     *  Let the user be responsible for normalizing their quaternion.
     */
    // *self = normalizeQuat((const Quat *const) self);
}

/** 
 *  Offset given Quaternion around given axis by given number
 *  of radians. Quaternion is not guaranteed to be normalized after this function
 *  is called.
 *  
 *  @param  self    Quat to operate on.
 *  @param  x       scalar, x-component of rotation axis.
 *  @param  y       scalar, y-component of rotation axis.
 *  @param  z       scalar, z-component of rotation axis.
 *  @param  angle   float, angle in radians to rotate.
 */
static void offsetAxisXYZ(struct Quat_s *const self, scalar x, scalar y, scalar z, float angle) {
    Vec3 axis = createVec3(x, y, z);
    Vec3 axisN = normalizeVec3(&axis);

    axisN = mulVec3Scalar(&axisN, sinf(angle / 2.0f));
    scalar scalar = cosf(angle / 2.0f);

    Quat offset = createQuatAxisScalar(&axisN, scalar);
    
    *self = mulQuatQuat(self, &offset);
}

/**
 *  Calculates the magnitude of the given Quaternion.
 *
 *  @param self     Quat to calculate magnitude of.
 *  @return         scalar, magnitude of given Quaternion.
 */
static scalar magnitudeQuat(const struct Quat_s *const self) {
    return ( sqrt(pow(self->x, 2) + pow(self->y, 2) + pow(self->z, 2) + pow(self->w, 2)) );
}