#include <math.h>

#include "Vec2.h"

static scalar magnitude(const Vec2 *const vec);

static Vec2 create(Vec2 *const vec, scalar x, scalar y) {
    Vec2 result;

    result.x = x;
    result.y = y;

    if (vec != NULL) {
        vec->x = x;
        vec->y = y;
    }

    return result;
}

static Vec2 createFromVec2(Vec2 *const vec, const Vec2 *const vec2) {
    return (
        manVec2.create(
            vec,
            vec2->x,
            vec2->y
        )
    );
}

static scalar magnitude(const Vec2 *const vec) {
    return (sqrt(pow(vec->x, 2) + pow(vec->y, 2)));
}

static Vec2 sum(const Vec2 *const v1, const Vec2 *const v2) {
    return (
        manVec2.create(
            NULL,
            v1->x + v2->x,
            v1->y + v2->y
        )
    );
}

static Vec2 sub(const Vec2 *const v1, const Vec2 *const v2) {
    return (
        manVec2.create(
            NULL,
            v1->x - v2->x,
            v1->y - v2->y
        )
    );
}

static Vec2 postMulScalar(const Vec2 *const vec, scalar factor) {
    return (
        manVec2.create(
            NULL,
            vec->x * factor,
            vec->y * factor
        )
    );
}

static Vec2 preMulScalar(scalar factor, const Vec2 *const vec) {
    return (
        manVec2.create(
            NULL,
            vec->x * factor,
            vec->y * factor
        )
    );    
}

static Vec2 normalize(const Vec2 *const vec) {
    scalar magnitude = manVec2.magnitude(vec);

    return (
        manVec2.create(
            NULL,
            vec->x / magnitude,
            vec->y / magnitude
        )
    );
}

static Vec2 invert(const Vec2 *const vec) {
    return (
        manVec2.create(
            NULL,
            -vec->x,
            -vec->y
        )
    );   
}

static scalar dot(const Vec2 *const v1, const Vec2 *const v2) {
   return (
        v1->x * v2->x +
        v1->y * v2->y
    ); 
}

const Vec2Manager manVec2 = {create, createFromVec2, sum, sub, postMulScalar, preMulScalar, normalize, invert, dot, magnitude};