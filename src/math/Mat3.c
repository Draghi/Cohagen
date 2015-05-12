#include "Mat3.h"

static Mat3 create(    Mat3 *const mat,
                scalar el00, scalar el10, scalar el20,
                scalar el01, scalar el11, scalar el21,
                scalar el02, scalar el12, scalar el22);
static Mat3 createLeading(Mat3 *const mat, scalar leading);
static Mat3 createFromVec3(Mat3 *const mat, const Vec3 *const v0, const Vec3 *const v1, const Vec3 *const v2);
static Mat3 createFromMat3(Mat3 *const mat, const Mat3 *const mat3);
static Mat3 sum(const Mat3 *const mat1, const Mat3 *const mat2);
static Mat3 sub(const Mat3 *const mat1, const Mat3 *const mat2);
static Mat3 mul(const Mat3 *const mat1, const Mat3 *const mat2);
static Vec3 postMulVec3(const Mat3 *const matrix, const Vec3 *const col);
static Vec3 preMulVec3(const Vec3 *const row, const Mat3 *const matrix);
static Mat3 postMulScalar(const Mat3 *const matrix, scalar factor);
static Mat3 preMulScalar(scalar factor, const Mat3 *const matrix);


static Mat3 create( Mat3 *const mat,
                scalar el00, scalar el10, scalar el20,
                scalar el01, scalar el11, scalar el21,
                scalar el02, scalar el12, scalar el22) {
    Mat3 matrix;

    matrix.data[0] = manVec3.create(NULL, el00, el01, el02);
    matrix.data[1] = manVec3.create(NULL, el10, el11, el12);
    matrix.data[2] = manVec3.create(NULL, el20, el21, el22);

    if (mat != NULL) {
        createFromMat3(mat, &matrix);
    }

    return matrix;
}

static Mat3 createLeading(Mat3 *const mat, scalar leading) {
    Mat3 matrix;

    matrix.data[0] = manVec3.create(NULL, leading, 0.0f, 0.0f);
    matrix.data[1] = manVec3.create(NULL, 0.0f, leading, 0.0f);
    matrix.data[2] = manVec3.create(NULL, 0.0f, 0.0f, leading);

    if (mat != NULL) {
        createFromMat3(mat, &matrix);
    }

    return matrix;
}

static Mat3 createFromVec3(Mat3 *const mat, const Vec3 *const v0, const Vec3 *const v1, const Vec3 *const v2) {
    Mat3 matrix;

    matrix.data[0] = *v0;
    matrix.data[1] = *v1;
    matrix.data[2] = *v2;

    if (mat != NULL) {
        createFromMat3(mat, &matrix);
    }

    return matrix;
}

static Mat3 createFromMat3(Mat3 *const mat, const Mat3 *const mat3) {   
    return (
        createFromVec3(
            mat,
            &(mat3->data[0]),
            &(mat3->data[1]),
            &(mat3->data[2])
        )
    );
}

static Mat3 sum(const Mat3 *const mat1, const Mat3 *const mat2) {
    Vec3 v0 = manVec3.sum(&(mat1->data[0]), &(mat2->data[0]));
    Vec3 v1 = manVec3.sum(&(mat1->data[1]), &(mat2->data[1]));
    Vec3 v2 = manVec3.sum(&(mat1->data[2]), &(mat2->data[2]));

    return (
        createFromVec3(
            NULL,
            &v0,
            &v1,
            &v2
        )
    );
}

static Mat3 sub(const Mat3 *const mat1, const Mat3 *const mat2) {
    Vec3 v0 = manVec3.sub(&(mat1->data[0]), &(mat2->data[0]));
    Vec3 v1 = manVec3.sub(&(mat1->data[1]), &(mat2->data[1]));
    Vec3 v2 = manVec3.sub(&(mat1->data[2]), &(mat2->data[2]));

    return (
        createFromVec3(
            NULL,
            &v0,
            &v1,
            &v2
        )
    );
}

static Mat3 mul(const Mat3 *const mat1, const Mat3 *const mat2) {
    Vec3 row0 = manVec3.create(NULL, mat1->data[0].x, mat1->data[1].x, mat1->data[2].x);
    Vec3 row1 = manVec3.create(NULL, mat1->data[0].y, mat1->data[1].y, mat1->data[2].y);
    Vec3 row2 = manVec3.create(NULL, mat1->data[0].z, mat1->data[1].z, mat1->data[2].z);
    Vec3 col0 = mat2->data[0];
    Vec3 col1 = mat2->data[1];
    Vec3 col2 = mat2->data[2];

    scalar x0 = manVec3.dot(&row0, &col0);
    scalar y0 = manVec3.dot(&row1, &col0);
    scalar z0 = manVec3.dot(&row2, &col0);
    scalar x1 = manVec3.dot(&row0, &col1);
    scalar y1 = manVec3.dot(&row1, &col1);
    scalar z1 = manVec3.dot(&row2, &col1);
    scalar x2 = manVec3.dot(&row0, &col2);
    scalar y2 = manVec3.dot(&row1, &col2);
    scalar z2 = manVec3.dot(&row2, &col2);

    Vec3 v0 = manVec3.create(NULL, x0, y0, z0);
    Vec3 v1 = manVec3.create(NULL, x1, y1, z1);
    Vec3 v2 = manVec3.create(NULL, x2, y2, z2);
    
    return (createFromVec3(NULL, &v0, &v1, &v2));
}

static Vec3 postMulVec3(const Mat3 *const matrix, const Vec3 *const col) {
    Vec3 row0 = manVec3.create(NULL, matrix->data[0].x, matrix->data[1].x, matrix->data[2].x);
    Vec3 row1 = manVec3.create(NULL, matrix->data[0].y, matrix->data[1].y, matrix->data[2].y);
    Vec3 row2 = manVec3.create(NULL, matrix->data[0].z, matrix->data[1].z, matrix->data[2].z);

    scalar x = manVec3.dot(&row0, col);
    scalar y = manVec3.dot(&row1, col);
    scalar z = manVec3.dot(&row2, col);

    return (manVec3.create(NULL, x, y, z));
}

static Vec3 preMulVec3(const Vec3 *const row, const Mat3 *const matrix) {
    Vec3 col0 = manVec3.create(NULL, matrix->data[0].x, matrix->data[1].x, matrix->data[2].x);
    Vec3 col1 = manVec3.create(NULL, matrix->data[0].y, matrix->data[1].y, matrix->data[2].y);
    Vec3 col2 = manVec3.create(NULL, matrix->data[0].z, matrix->data[1].z, matrix->data[2].z);

    scalar x = manVec3.dot(&col0, row);
    scalar y = manVec3.dot(&col1, row);
    scalar z = manVec3.dot(&col2, row);

    return (manVec3.create(NULL, x, y, z));
}

static Mat3 postMulScalar(const Mat3 *const matrix, scalar factor) {
    Vec3 col0 = manVec3.postMulScalar(&(matrix->data[0]), factor);
    Vec3 col1 = manVec3.postMulScalar(&(matrix->data[1]), factor);
    Vec3 col2 = manVec3.postMulScalar(&(matrix->data[2]), factor);

    return (
        createFromVec3(
            NULL,
            &col0,
            &col1,
            &col2
        )
    );
}

static Mat3 preMulScalar(scalar factor, const Mat3 *const matrix) {
    Vec3 col0 = manVec3.preMulScalar(factor, &(matrix->data[0]));
    Vec3 col1 = manVec3.preMulScalar(factor, &(matrix->data[1]));
    Vec3 col2 = manVec3.preMulScalar(factor, &(matrix->data[2]));

    return (
        createFromVec3(
            NULL,
            &col0,
            &col1,
            &col2
        )
    );
}

const Mat3Manager manMat3 = {create, createLeading, createFromVec3, createFromMat3, sum, sub, mul, postMulVec3, preMulVec3, postMulScalar, preMulScalar};
