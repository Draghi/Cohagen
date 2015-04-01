#include "Quat.h"

#include <math.h>

static void offsetAxis(Quat *const quat, const Vec3 *const axis, float angleRad);
static void offsetAxisXYZ(Quat *const quat, scalar x, scalar y, scalar z, float angleRad);
static scalar magnitude(const Quat *const self);

static Quat create(Quat *const quat, scalar x, scalar y, scalar z, scalar w) {
    Quat quaternion;

    quaternion.x = x;
    quaternion.y = y;
    quaternion.z = z;
    quaternion.w = w;

    if (quat != NULL) {
        quat->x = x;
        quat->y = y;
        quat->z = z;
        quat->w = w;
    }

    return quaternion;
}

static Quat createFromAxisScalar(Quat *const quat, const Vec3 *const axis, scalar scalar) {
    return (
        manQuat.create(
            quat,
            axis->x,
            axis->y,
            axis->z,
            scalar
        )
    );
}

static Quat invert(const Quat *const quat) {
    return (
        manQuat.create(
            NULL,
            quat->x,
            quat->y,
            quat->z,
            -quat->w
        )
    );
}


static Quat mul(const Quat *const q1, const Quat *const q2) {
    return (
        manQuat.create(
            NULL,   
            q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y,
            q1->w * q2->y + q1->y * q2->w + q1->z * q2->x - q1->x * q2->z,
            q1->w * q2->z + q1->z * q2->w + q1->x * q2->y - q1->y * q2->x,
            q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z
        )
    );
}

static Quat normalize(const Quat *const quat) {
    scalar magnitude = manQuat.magnitude(quat);

    return (
        manQuat.create(
            NULL,
            quat->x / magnitude,
            quat->y / magnitude,
            quat->z / magnitude,
            quat->w / magnitude
        )
    );
}

static scalar dot(const Quat *const q1, const Quat *const q2) {
    return (
        q1->x * q2->x + 
        q1->y * q2->y +
        q1->z * q2->z +
        q1->w * q2->w
    );
}

static Mat4 castMat4(const Quat *const quat) {
    // Mat4f columns
    Vec4 col0 = manVec4.create(NULL, 1 - 2*quat->y*quat->y - 2*quat->z*quat->z, 2*quat->x*quat->y + 2*quat->w*quat->z, 2*quat->x*quat->z - 2*quat->w*quat->y, 0.0f);
    Vec4 col1 = manVec4.create(NULL, 2*quat->x*quat->y - 2*quat->w*quat->z, 1 - 2*quat->x*quat->x - 2*quat->z*quat->z, 2*quat->y*quat->z + 2*quat->w*quat->x, 0.0f);
    Vec4 col2 = manVec4.create(NULL, 2*quat->x*quat->z + 2*quat->w*quat->y, 2*quat->y*quat->z - 2*quat->w*quat->x, 1 - 2*quat->x*quat->x - 2*quat->y*quat->y, 0.0f);
    Vec4 col3 = manVec4.create(NULL, 0.0f, 0.0f, 0.0f, 1.0f);
        
    return ( manMat4.createFromVec4(NULL, &col0, &col1, &col2, &col3) );
}

static void offsetAxis(Quat *const quat, const Vec3 *const axis, float angleRad) {
    Vec3 axisN = manVec3.normalize(axis);

    axisN = manVec3.postMulScalar(&axisN, sinf(angleRad / 2.0f));
    scalar scalar = cosf(angleRad / 2.0f);

    Quat offset = manQuat.createFromAxisScalar(NULL, &axisN, scalar);
    
    *quat = manQuat.mul(quat, &offset);

    /*
     *  I've commented this line out because trying to normalize a (0, 0, 0, 0) quaternion will cause a divide by zero error.
     *  Let the user be responsible for normalizing their quaternion.
     */
    // *quat = normalizeQuat((const Quat *const) quat);
}

static void offsetAxisXYZ(Quat *const quat, scalar x, scalar y, scalar z, float angleRad) {
    Vec3 axis = manVec3.create(NULL, x, y, z);
    Vec3 axisN = manVec3.normalize(&axis);

    axisN = manVec3.postMulScalar(&axisN, sinf(angleRad / 2.0f));
    scalar scalar = cosf(angleRad / 2.0f);

    Quat offset = manQuat.createFromAxisScalar(NULL, &axisN, scalar);
    
    *quat = manQuat.mul(quat, &offset);
}

static scalar magnitude(const Quat *const self) {
    return ( sqrt(pow(self->x, 2) + pow(self->y, 2) + pow(self->z, 2) + pow(self->w, 2)) );
}

const QuatManager manQuat = {create, createFromAxisScalar, invert, mul, normalize, dot, castMat4, offsetAxis, offsetAxisXYZ, magnitude};
