#include "Mat4.h"

#include <math.h>

static Mat4 create(Mat4 *const mat,
            scalar el00, scalar el10, scalar el20, scalar el30, 
            scalar el01, scalar el11, scalar el21, scalar el31,
            scalar el02, scalar el12, scalar el22, scalar el32,
            scalar el03, scalar el13, scalar el23, scalar el33) {
    Mat4 matrix;

    matrix.data[0] = manVec4.create(NULL, el00, el01, el02, el03);
    matrix.data[1] = manVec4.create(NULL, el10, el11, el12, el13);
    matrix.data[2] = manVec4.create(NULL, el20, el21, el22, el23);
    matrix.data[3] = manVec4.create(NULL, el30, el31, el32, el33);

    if (mat != NULL) {
        mat->data[0] = manVec4.create(NULL, el00, el01, el02, el03);
        mat->data[1] = manVec4.create(NULL, el10, el11, el12, el13);
        mat->data[2] = manVec4.create(NULL, el20, el21, el22, el23);
        mat->data[3] = manVec4.create(NULL, el30, el31, el32, el33);
    }

    return matrix;
}

static Mat4 createLeading(Mat4 *const mat, scalar leading) {
    Mat4 matrix;

    matrix.data[0] = manVec4.create(NULL, leading, 0.0f, 0.0f, 0.0f);
    matrix.data[1] = manVec4.create(NULL, 0.0f, leading, 0.0f, 0.0f);
    matrix.data[2] = manVec4.create(NULL, 0.0f, 0.0f, leading, 0.0f);
    matrix.data[3] = manVec4.create(NULL, 0.0f, 0.0f, 0.0f, leading);

    if (mat != NULL) {
        mat->data[0] = manVec4.create(NULL, leading, 0.0f, 0.0f, 0.0f);
        mat->data[1] = manVec4.create(NULL, 0.0f, leading, 0.0f, 0.0f);
        mat->data[2] = manVec4.create(NULL, 0.0f, 0.0f, leading, 0.0f);
        mat->data[3] = manVec4.create(NULL, 0.0f, 0.0f, 0.0f, leading);
    }

    return matrix;
}

static Mat4 createFromVec4(Mat4 *const mat, const Vec4 *const v0, const Vec4 *const v1, const Vec4 *const v2, const Vec4 *const v3) {
    Mat4 matrix;

    matrix.data[0] = *v0;
    matrix.data[1] = *v1;
    matrix.data[2] = *v2;
    matrix.data[3] = *v3;

    if (mat != NULL) {
        mat->data[0] = *v0;
        mat->data[1] = *v1;
        mat->data[2] = *v2;
        mat->data[3] = *v3;
    }

    return matrix;
}

static Mat4 createFromMat4(Mat4 *const mat, const Mat4 *const matrix) {
    return (
        manMat4.createFromVec4(
            mat,
            &(matrix->data[0]),
            &(matrix->data[1]),
            &(matrix->data[2]),
            &(matrix->data[3])
        )
    );
}

static Mat4 sum(const Mat4 *const mat1, const Mat4 *const mat2) {
    Vec4 v0 = manVec4.sum(&(mat1->data[0]), &(mat2->data[0]));
    Vec4 v1 = manVec4.sum(&(mat1->data[1]), &(mat2->data[1]));
    Vec4 v2 = manVec4.sum(&(mat1->data[2]), &(mat2->data[2]));
    Vec4 v3 = manVec4.sum(&(mat1->data[3]), &(mat2->data[3]));

    return (
        manMat4.createFromVec4(
            NULL,
            &v0,
            &v1,
            &v2,
            &v3
        )
    );
}

static Mat4 sub(const Mat4 *const mat1, const Mat4 *const mat2) {
    Vec4 v0 = manVec4.sum(&(mat1->data[0]), &(mat2->data[0]));
    Vec4 v1 = manVec4.sum(&(mat1->data[1]), &(mat2->data[1]));
    Vec4 v2 = manVec4.sum(&(mat1->data[2]), &(mat2->data[2]));
    Vec4 v3 = manVec4.sum(&(mat1->data[3]), &(mat2->data[3]));

    return (
        manMat4.createFromVec4(
            NULL,
            &v0,
            &v1,
            &v2,
            &v3
        )
    );
}

static Mat4 postMulScalar(const Mat4 *const matrix, scalar factor) {
    Vec4 col0 = manVec4.postMulScalar(&(matrix->data[0]), factor);
    Vec4 col1 = manVec4.postMulScalar(&(matrix->data[1]), factor);
    Vec4 col2 = manVec4.postMulScalar(&(matrix->data[2]), factor);
    Vec4 col3 = manVec4.postMulScalar(&(matrix->data[3]), factor);

    return (
        manMat4.createFromVec4(
            NULL,
            &col0,
            &col1,
            &col2,
            &col3
        )
    );
}

static Mat4 preMulScalar(scalar factor, const Mat4 *const matrix) {
    Vec4 col0 = manVec4.preMulScalar(factor, &(matrix->data[0]));
    Vec4 col1 = manVec4.preMulScalar(factor, &(matrix->data[1]));
    Vec4 col2 = manVec4.preMulScalar(factor, &(matrix->data[2]));
    Vec4 col3 = manVec4.preMulScalar(factor, &(matrix->data[3]));

    return (
        manMat4.createFromVec4(
            NULL,
            &col0,
            &col1,
            &col2,
            &col3
        )
    );
}

static Mat4 mul(const Mat4 *const mat1, const Mat4 *const mat2) {
    Vec4 row0 = manVec4.create(NULL, mat1->data[0].x, mat1->data[1].x, mat1->data[2].x, mat1->data[3].x);
    Vec4 row1 = manVec4.create(NULL, mat1->data[0].y, mat1->data[1].y, mat1->data[2].y, mat1->data[3].y);
    Vec4 row2 = manVec4.create(NULL, mat1->data[0].z, mat1->data[1].z, mat1->data[2].z, mat1->data[3].z);
    Vec4 row3 = manVec4.create(NULL, mat1->data[0].w, mat1->data[1].w, mat1->data[2].w, mat1->data[3].w);

    Vec4 col0 = mat2->data[0];
    Vec4 col1 = mat2->data[1];
    Vec4 col2 = mat2->data[2];
    Vec4 col3 = mat2->data[3];

    scalar x0 = manVec4.dot(&row0, &col0);
    scalar y0 = manVec4.dot(&row1, &col0);
    scalar z0 = manVec4.dot(&row2, &col0);
    scalar w0 = manVec4.dot(&row3, &col0);
    scalar x1 = manVec4.dot(&row0, &col1);
    scalar y1 = manVec4.dot(&row1, &col1);
    scalar z1 = manVec4.dot(&row2, &col1);
    scalar w1 = manVec4.dot(&row3, &col1);
    scalar x2 = manVec4.dot(&row0, &col2);
    scalar y2 = manVec4.dot(&row1, &col2);
    scalar z2 = manVec4.dot(&row2, &col2);
    scalar w2 = manVec4.dot(&row3, &col2);
    scalar x3 = manVec4.dot(&row0, &col3);
    scalar y3 = manVec4.dot(&row1, &col3);
    scalar z3 = manVec4.dot(&row2, &col3);
    scalar w3 = manVec4.dot(&row3, &col3);

    Vec4 v0 = manVec4.create(NULL, x0, y0, z0, w0);
    Vec4 v1 = manVec4.create(NULL, x1, y1, z1, w1);
    Vec4 v2 = manVec4.create(NULL, x2, y2, z2, w2);
    Vec4 v3 = manVec4.create(NULL, x3, y3, z3, w3);

    return (
        manMat4.createFromVec4(
            NULL,
            &v0,
            &v1,
            &v2,
            &v3
        )
    );
}

static Vec4 postMulVec4(const Mat4 *const matrix, const Vec4 *const col) {
    Vec4 row0 = manVec4.create(NULL, matrix->data[0].x, matrix->data[1].x, matrix->data[2].x, matrix->data[3].x);
    Vec4 row1 = manVec4.create(NULL, matrix->data[0].y, matrix->data[1].y, matrix->data[2].y, matrix->data[3].y);
    Vec4 row2 = manVec4.create(NULL, matrix->data[0].z, matrix->data[1].z, matrix->data[2].z, matrix->data[3].z);
    Vec4 row3 = manVec4.create(NULL, matrix->data[0].w, matrix->data[1].w, matrix->data[2].w, matrix->data[3].w);

    return (
        manVec4.create(
            NULL, 
            manVec4.dot(&row0, col),
            manVec4.dot(&row1, col),
            manVec4.dot(&row2, col),
            manVec4.dot(&row3, col)
        )
    );
}

static Vec4 preMulVec4(const Vec4 *const row, const Mat4 *const matrix) {
    Vec4 col0 = manVec4.create(NULL, matrix->data[0].x, matrix->data[0].y, matrix->data[0].z, matrix->data[0].w);
    Vec4 col1 = manVec4.create(NULL, matrix->data[1].x, matrix->data[1].y, matrix->data[1].z, matrix->data[1].w);
    Vec4 col2 = manVec4.create(NULL, matrix->data[2].x, matrix->data[2].y, matrix->data[2].z, matrix->data[2].w);
    Vec4 col3 = manVec4.create(NULL, matrix->data[3].x, matrix->data[3].y, matrix->data[3].z, matrix->data[3].w);

    return (
        manVec4.create(
            NULL, 
            manVec4.dot(row, &col0),
            manVec4.dot(row, &col1),
            manVec4.dot(row, &col2),
            manVec4.dot(row, &col3)
        )
    );
}

static Mat4 inverse(const Mat4 *const matrix) {
    // Uses Laplace expansion to find determinant and inverse of matrix
    // Find sub-factors (col, row).
    /* Sub-factor is determinant of 2x2 matrix left when:
     *  -you eliminate row and col of 4x4 co-factor 
     *  -you eliminate row and col of 3x3 co-factor
     */
    float sub00 = matrix->data[2].z*matrix->data[3].w - matrix->data[3].z*matrix->data[2].w;
    float sub01 = matrix->data[1].z*matrix->data[3].w - matrix->data[3].z*matrix->data[1].w;
    float sub02 = matrix->data[1].z*matrix->data[2].w - matrix->data[2].z*matrix->data[1].w;
    float sub03 = matrix->data[0].z*matrix->data[3].w - matrix->data[3].z*matrix->data[0].w;
    float sub04 = matrix->data[0].z*matrix->data[2].w - matrix->data[2].z*matrix->data[0].w;
    float sub05 = matrix->data[0].z*matrix->data[1].w - matrix->data[1].z*matrix->data[0].w;
    float sub06 = matrix->data[1].y*matrix->data[2].w - matrix->data[2].y*matrix->data[1].w;
    float sub07 = matrix->data[0].y*matrix->data[3].w - matrix->data[3].y*matrix->data[0].w;
    float sub08 = matrix->data[0].y*matrix->data[1].w - matrix->data[1].y*matrix->data[0].w;
    float sub10 = matrix->data[0].y*matrix->data[1].z - matrix->data[1].y*matrix->data[0].z;
    float sub11 = matrix->data[0].y*matrix->data[2].z - matrix->data[2].y*matrix->data[0].z;
    float sub12 = matrix->data[2].y*matrix->data[3].w - matrix->data[3].y*matrix->data[2].w;
    float sub13 = matrix->data[1].y*matrix->data[3].w - matrix->data[3].y*matrix->data[1].w;
    float sub14 = matrix->data[1].y*matrix->data[2].w - matrix->data[2].y*matrix->data[1].w;
    float sub15 = matrix->data[0].y*matrix->data[2].w - matrix->data[2].y*matrix->data[0].w;
    float sub16 = matrix->data[2].y*matrix->data[3].z - matrix->data[3].y*matrix->data[2].z;
    float sub17 = matrix->data[1].y*matrix->data[3].z - matrix->data[3].y*matrix->data[1].z;
    float sub18 = matrix->data[1].y*matrix->data[2].z - matrix->data[2].y*matrix->data[1].z;
    float sub19 = matrix->data[0].y*matrix->data[3].z - matrix->data[3].y*matrix->data[0].z;

    // Find co-factor matrix (col, row)
    float cof00 =     matrix->data[1].y*(sub00) - matrix->data[2].y*(sub01) + matrix->data[3].y*(sub02);
    float cof10 =   -(matrix->data[0].y*(sub00) - matrix->data[2].y*(sub03) + matrix->data[3].y*(sub04));
    float cof20 =     matrix->data[0].y*(sub01) - matrix->data[1].y*(sub03) + matrix->data[3].y*(sub05);
    float cof30 =   -(matrix->data[0].y*(sub02) - matrix->data[1].y*(sub04) + matrix->data[2].y*(sub05));

    float cof01 =   -(matrix->data[1].x*(sub00) - matrix->data[2].x*(sub01) + matrix->data[3].x*(sub02));
    float cof11 =     matrix->data[0].x*(sub00) - matrix->data[2].x*(sub03) + matrix->data[3].x*(sub04);
    float cof21 =   -(matrix->data[0].x*(sub01) - matrix->data[1].x*(sub03) + matrix->data[3].x*(sub05));
    float cof31 =     matrix->data[0].x*(sub02) - matrix->data[1].x*(sub04) + matrix->data[2].x*(sub05);

    float cof02 =     matrix->data[1].x*(sub12) - matrix->data[2].x*(sub13) + matrix->data[3].x*(sub14);
    float cof12 =   -(matrix->data[0].x*(sub12) - matrix->data[2].x*(sub07) + matrix->data[3].x*(sub15));
    float cof22 =     matrix->data[0].x*(sub13) - matrix->data[1].x*(sub07) + matrix->data[3].x*(sub08);
    float cof32 =   -(matrix->data[0].x*(sub06) - matrix->data[1].x*(sub15) + matrix->data[2].x*(sub08));

    float cof03 =   -(matrix->data[1].x*(sub16) - matrix->data[2].x*(sub17) + matrix->data[3].x*(sub18));
    float cof13 =     matrix->data[0].x*(sub16) - matrix->data[2].x*(sub19) + matrix->data[3].x*(sub11);

    float cof23 =   -(matrix->data[0].x*(sub17) - matrix->data[1].x*(sub19) + matrix->data[3].x*(sub10));
    float cof33 =     matrix->data[0].x*(sub18) - matrix->data[1].x*(sub11) + matrix->data[2].x*(sub10);

    // Transpose co-factor matrix to get adjoint matrix (note row-major order of constructor)
    Mat4 adj = manMat4.create(   
                    NULL,
                    cof00, cof01, cof02, cof03,
                    cof10, cof11, cof12, cof13,
                    cof20, cof21, cof22, cof23,
                    cof30, cof31, cof32, cof33
    );

    // Multiply adjoint matrix by inverse of determinant to get inverse matrix
    float det = matrix->data[0].x*cof00 + matrix->data[1].x*cof10 + matrix->data[2].x*cof20 + matrix->data[3].x*cof30;

    Mat4 inv = manMat4.postMulScalar(&adj, (1/det));

    return (inv);
}

static void getMat4Data(const Mat4 *const matrix, scalar *const data)
{
    // Load each column of matrix into data array
    int j = 0;
    for (int i = 0; i < 16; i = i + 4)
    {
        data[i] = (matrix->data[j].x);
        data[i + 1] = (matrix->data[j].y);
        data[i + 2] = (matrix->data[j].z);
        data[i + 3] = (matrix->data[j].w);
        ++j;
    }
}

static Mat4 affScale(const Mat4 *const matrix, const Vec3 *const scale) {
	Mat4 dest = *matrix;

	dest.data[0].x *= scale->x;
	dest.data[1].x *= scale->x;
	dest.data[2].x *= scale->x;
    dest.data[3].x *= scale->x;

	dest.data[0].y *= scale->y;
	dest.data[1].y *= scale->y;
	dest.data[2].y *= scale->y;
	dest.data[3].y *= scale->y;

	dest.data[0].z *= scale->z;
	dest.data[1].z *= scale->z;
	dest.data[2].z *= scale->z;
	dest.data[3].z *= scale->z;

	return dest;
}

static Mat4 affTranslate(const Mat4 *const matrix, const Vec3 *const translation) {
	Mat4 dest = *matrix;

	dest.data[3].x += matrix->data[0].x * -translation->x + matrix->data[1].x * translation->y + matrix->data[2].x * -translation->z;
	dest.data[3].y += matrix->data[0].y * -translation->x + matrix->data[1].y * translation->y + matrix->data[2].y * -translation->z;
	dest.data[3].z += matrix->data[0].z * -translation->x + matrix->data[1].z * translation->y + matrix->data[2].z * -translation->z;
	dest.data[3].w += matrix->data[0].w * -translation->x + matrix->data[1].w * translation->y + matrix->data[2].w * -translation->z;

	return dest;
}

static Mat4 affRotate(const Mat4 *const matrix, const scalar angle, const Vec3 *const axis) {
	Mat4 dest = *matrix;

	//Pre-calculations to streamline matrix transformation.
	scalar aCos = (scalar) cos(angle);
	scalar aSin = (scalar) sin(angle);
	scalar invC = 1.0f - aCos;

	scalar xx = axis->x*axis->x*invC;
	scalar yy = axis->y*axis->y*invC;
	scalar zz = axis->z*axis->z*invC;

	scalar xy = axis->x*axis->y*invC;
	scalar yz = axis->y*(-axis->z)*invC;
	scalar xz = axis->x*(-axis->z)*invC;

	scalar xsin = axis->x*aSin;
	scalar ysin = axis->y*aSin;
	scalar zsin = (-axis->z)*aSin;


	//Transform matrix by doing an explicit Vec3 multiply to each component.
	//Explicit so we avoid the overhead of function calls. We want this to be very very fast.
	scalar m00 = xx + aCos;
	scalar m01 = xy + zsin;
	scalar m02 = xz - ysin;
	dest.data[0].x = matrix->data[0].x * m00 + matrix->data[1].x * m01 + matrix->data[2].x * m02;
	dest.data[0].y = matrix->data[0].y * m00 + matrix->data[1].y * m01 + matrix->data[2].y * m02;
	dest.data[0].z = matrix->data[0].z * m00 + matrix->data[1].z * m01 + matrix->data[2].z * m02;
	dest.data[0].w = matrix->data[0].w * m00 + matrix->data[1].w * m01 + matrix->data[2].w * m02;

	scalar m10 = xy - zsin;
	scalar m11 = yy + aCos;
	scalar m12 = yz + xsin;
	dest.data[1].x = matrix->data[0].x * m10 + matrix->data[1].x * m11 + matrix->data[2].x * m12;
	dest.data[1].y = matrix->data[0].y * m10 + matrix->data[1].y * m11 + matrix->data[2].y * m12;
	dest.data[1].z = matrix->data[0].z * m10 + matrix->data[1].z * m11 + matrix->data[2].z * m12;
	dest.data[1].w = matrix->data[0].w * m10 + matrix->data[1].w * m11 + matrix->data[2].w * m12;

	scalar m20 = xz + ysin;
	scalar m21 = yz - xsin;
	scalar m22 = zz + aCos;
	dest.data[2].x = matrix->data[0].x * m20 + matrix->data[1].x * m21 + matrix->data[2].x * m22;
	dest.data[2].y = matrix->data[0].y * m20 + matrix->data[1].y * m21 + matrix->data[2].y * m22;
	dest.data[2].z = matrix->data[0].z * m20 + matrix->data[1].z * m21 + matrix->data[2].z * m22;
	dest.data[2].w = matrix->data[0].w * m20 + matrix->data[1].w * m21 + matrix->data[2].w * m22;

	return dest;
}

const Mat4Manager manMat4 = {create, createLeading, createFromVec4, createFromMat4, sum, sub, postMulScalar, preMulScalar, mul, postMulVec4, preMulVec4, inverse, getMat4Data, affScale, affTranslate, affRotate};
