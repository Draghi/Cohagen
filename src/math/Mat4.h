#ifndef MATH_MAT4_H
#define MATH_MAT4_H

#include "Vec4.h"

typedef struct Mat4_s {
    Vec4 data[4];
} Mat4;

Mat4 createMat4(        scalar el00, scalar el10, scalar el20, scalar el30, 
                        scalar el01, scalar el11, scalar el21, scalar el31,
                        scalar el02, scalar el12, scalar el22, scalar el32,
                        scalar el03, scalar el13, scalar el23, scalar el33);
Mat4 createMat4Leading(scalar leading);
Mat4 createMat4Vec4(const Vec4 *const v0, const Vec4 *const v1, const Vec4 *const v2, const Vec4 *const v3);
Mat4 createMat4Mat4(const Mat4 *const matrix);

Mat4 sumMat4Mat4(const Mat4 *const mat1, const Mat4 *const mat2);
Mat4 subMat4Mat4(const Mat4 *const mat1, const Mat4 *const mat2);
Mat4 mulMat4Scalar(const Mat4 *const matrix, scalar factor);
Mat4 mulMat4Mat4(const Mat4 *const mat1, const Mat4 *const mat2);
Vec4 mulMat4Vec4(const Mat4 *const matrix, const Vec4 *const row);
Vec4 mulVec4Mat4(const Vec4 *const col, const Mat4 *const matrix);
Mat4 inverseMat4(const Mat4 *const matrix);

#endif