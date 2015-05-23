#include "Vec3.h"

#include <math.h>
#include <assert.h>

static scalar magnitude(const Vec3 *const vec);

static Vec3 create(Vec3 *const vec, scalar x, scalar y, scalar z) {
    Vec3 result;

    result.x = x;
    result.y = y;
    result.z = z;

    if (vec != NULL) {
        vec->x = x;
        vec->y = y;
        vec->z = z;
    }

    return result;
}

static Vec3 createFromVec3(Vec3 *const vec, const Vec3 *const vec3) {
    return (
        manVec3.create(
            vec,
            vec3->x,
            vec3->y,
            vec3->z
        )
    );
}

static Vec3 sum(const Vec3 *const v1, const Vec3 *const v2) {
    return (
        manVec3.create(
            NULL,
            v1->x + v2->x,
            v1->y + v2->y,
            v1->z + v2->z
        )
    );
}

static Vec3 sub(const Vec3 *const v1, const Vec3 *const v2) {
   return (
        manVec3.create(
            NULL,
            v1->x - v2->x,
            v1->y - v2->y,
            v1->z - v2->z
        )
    );
}

static Vec3 postMulScalar(const Vec3 *const vec, scalar factor) {
    return (
        manVec3.create(
            NULL,
            factor * vec->x,
            factor * vec->y,
            factor * vec->z
        )
    );
}

static Vec3 preMulScalar(scalar factor, const Vec3 *const vec) {
    return (
        manVec3.create(
            NULL,
            factor * vec->x,
            factor * vec->y,
            factor * vec->z
        )
    );
}

static Vec3 invert(const Vec3 *const vec) {
    return (
        manVec3.create(
            NULL,
            -vec->x,
            -vec->y,
            -vec->z
        )
    );
}

static scalar dot(const Vec3 *const v1, const Vec3 *const v2) {
    return (
        v1->x * v2->x +
        v1->y * v2->y +
        v1->z * v2->z
    );
}

static Vec3 cross(const Vec3 *const v1, const Vec3 *const v2) {
    return (
        manVec3.create(
            NULL,
            v1->y * v2->z - v1->z * v2->y,
            v1->z * v2->x - v1->x * v2->z,
            v1->x * v2->y - v1->y * v2->x
        )
    );
}

static Vec3 normalize(const Vec3 *const vec) {
    assert(vec != NULL && "Vector is NULL!");
    scalar magnitude = manVec3.magnitude(vec);

    if (magnitude == 0) {
        return *vec;
    } else {
        return (
            manVec3.create(
                NULL,
                vec->x / magnitude,
                vec->y / magnitude,
                vec->z / magnitude
            )
        );
    }
}

static scalar magnitude(const Vec3 *const vec) {
    return (sqrt(pow(vec->x, 2) + pow(vec->y, 2) + pow(vec->z, 2)));
}

const Vec3Manager manVec3 = {create, createFromVec3, sum, sub, postMulScalar, preMulScalar, invert, dot, cross, normalize, magnitude};
