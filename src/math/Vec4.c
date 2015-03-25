#include <math.h>

#include "Vec4.h"

static scalar magnitude(const Vec4 *const vec);

static Vec4 create(Vec4 *const vec, scalar x, scalar y, scalar z, scalar w) {
    Vec4 result;

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;

    if (vec != NULL) {
        vec->x = x;
        vec->y = y;
        vec->z = z;
        vec->w = w;
    }

    return (result);
}

static Vec4 createFromVec3(Vec4 *const vec, const Vec3 *const vec3, scalar w) {
    return (
        manVec4.create(
            vec,
            vec3->x,
            vec3->y,
            vec3->z,
            w
        )
    );
}

static Vec4 createFromVec4(Vec4 *const vec, const Vec4 *const vec4) {
    return (
        manVec4.create(
            vec,
            vec->x,
            vec->y,
            vec->z,
            vec->w
        )
    );
}

static Vec4 sum(const Vec4 *const v1, const Vec4 *const v2) {
    return (
        manVec4.create(
            NULL,
            v1->x + v2->x,
            v1->y + v2->y,
            v1->z + v2->z,
            v1->w + v2->w
        )
    );
}

static Vec4 sub(const Vec4 *const v1, const Vec4 *const v2) {
    return (
        manVec4.create(
            NULL,
            v1->x - v2->x,
            v1->y - v2->y,
            v1->z - v2->z,
            v1->w - v2->w
        )
    );
}

static Vec4 postMulScalar(const Vec4 *const vec, scalar factor) {
    return (
        manVec4.create(
            NULL,
            vec->x * factor,
            vec->y * factor,
            vec->z * factor,
            vec->w * factor
        )
    );
}

static Vec4 preMulScalar(scalar factor, const Vec4 *const vec) {
    return (
        manVec4.create(
            NULL,
            vec->x * factor,
            vec->y * factor,
            vec->z * factor,
            vec->w * factor
        )
    );
}

static Vec4 invert(const Vec4 *const vec) {
    return (
        manVec4.create(
            NULL,
            -vec->x,
            -vec->y,
            -vec->z,
            -vec->w
        )
    );
}

static scalar dot(const Vec4 *const v1, const Vec4 *const v2) {
    return (
        v1->x * v2->x +
        v1->y * v2->y +
        v1->z * v2->z +
        v1->w * v2->w
    );
}

static Vec4 normalize(const Vec4 *const vec) {
    scalar magnitude = manVec4.magnitude(vec);
    
    return (
        manVec4.create(
            NULL,
            vec->x / magnitude,
            vec->y / magnitude,
            vec->z / magnitude,
            vec->w / magnitude
        )
    );
}


static scalar magnitude(const Vec4 *const vec) {
    return (sqrt(pow(vec->x, 2) + pow(vec->y, 2) + pow(vec->z, 2) + pow(vec->w, 2)));
}

const Vec4Manager manVec4 = {create, createFromVec3, createFromVec4, sum, sub, postMulScalar, preMulScalar, invert, dot, normalize, magnitude};
