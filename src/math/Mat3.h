#ifndef MATH_MAT3_H
#define MATH_MAT3_H

#include "Precision.h"
#include "Vec3.h"

typedef struct Mat3_s {
    Vec3 data[3];
} Mat3;

Mat3 createMat3( scalar el00, scalar el10, scalar el20,
                        scalar el01, scalar el11, scalar el21,
                        scalar el02, scalar el12, scalar el22);
Mat3 createMat3Leading(scalar leading);
Mat3 createMat3Vec3(const Vec3 *const v0, const Vec3 *const v1, const Vec3 *const v2);
Mat3 createMat3Mat3(const Mat3 *const matrix);

Mat3 sumMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2);
Mat3 subMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2);
Mat3 mulMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2);
Vec3 mulMat3Vec3(const Mat3 *const matrix, const Vec3 *const vec);
Mat3 inverseMat3(const Mat3 *const matrix);

#endif