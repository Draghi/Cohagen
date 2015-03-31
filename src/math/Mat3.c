#include "Mat3.h"

Mat3 createMat3(    scalar el00, scalar el10, scalar el20,
                    scalar el01, scalar el11, scalar el21,
                    scalar el02, scalar el12, scalar el22) {
    Mat3 matrix;

    matrix.data[0] = manVec3.create(NULL, el00, el01, el02);
    matrix.data[1] = manVec3.create(NULL, el10, el11, el12);
    matrix.data[2] = manVec3.create(NULL, el20, el21, el22);

    return matrix;
}

Mat3 createMat3Leading(scalar leading) {
    Mat3 matrix;

    matrix.data[0] = manVec3.create(NULL, leading, 0.0f, 0.0f);
    matrix.data[1] = manVec3.create(NULL, 0.0f, leading, 0.0f);
    matrix.data[2] = manVec3.create(NULL, 0.0f, 0.0f, leading);

    return matrix;
}

Mat3 createMat3Vec3(const Vec3 *const v0, const Vec3 *const v1, const Vec3 *const v2) {
    Mat3 matrix;

    matrix.data[0] = *v0;
    matrix.data[1] = *v1;
    matrix.data[2] = *v2;

    return matrix;
}

Mat3 createMat3Mat3(const Mat3 *const matrix) {
    return (
        createMat3Vec3(
            &(matrix->data[0]),
            &(matrix->data[1]),
            &(matrix->data[2])
        )
    );
}

Mat3 sumMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2) {
    Vec3 v0 = manVec3.sum(&(mat1->data[0]), &(mat2->data[0]));
    Vec3 v1 = manVec3.sum(&(mat1->data[1]), &(mat2->data[1]));
    Vec3 v2 = manVec3.sum(&(mat1->data[2]), &(mat2->data[2]));

    return (
        createMat3Vec3(
            &v0,
            &v1,
            &v2
        )
    );
}

/**
 *  Return a Mat3 that is the result of the component-wise subtraction of
 *  mat2 from mat1. (ie. mat1 - mat2).
 *
 *  @param mat1     const pointer to const matrix 1.
 *  @param mat2     const pointer to const matrix 2.
 *  @return         Mat3 equal to mat1 - mat2 (component-wise subtraction).
 */
Mat3 subMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2) {
    Vec3 v0 = manVec3.sub(&(mat1->data[0]), &(mat2->data[0]));
    Vec3 v1 = manVec3.sub(&(mat1->data[1]), &(mat2->data[1]));
    Vec3 v2 = manVec3.sub(&(mat1->data[2]), &(mat2->data[2]));

    return (
        createMat3Vec3(
            &v0,
            &v1,
            &v2
        )
    );
}

/**
 *  Return a Mat3 equal to the result of the matrix multiplication operation
 *  between mat1 and mat2. (ie. mat1 * mat2).
 *
 *  @param mat1     const pointer to const matrix 1.
 *  @param mat2     const pointer to const matrix 2.
 *  @return         Mat3 equal to the result of the matrix multiplication operation between
 *                  mat1 and mat2.
 */
Mat3 mulMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2) {
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
    
    return (createMat3Vec3(&v0, &v1, &v2));
}

/**
 *  Return a Vec3 equal to the result of multiplying the given Mat3 and the given Vec3 column vector.
 *  (ie. matrix * col).
 *
 *  @param  matrix  const pointer to const Mat3.
 *  @param  col     const pointer to const Vec3, column-vector.
 *  @return         Vec3 equal to result of matrix * col.
 */
Vec3 mulMat3Vec3(const Mat3 *const matrix, const Vec3 *const col) {
    Vec3 row0 = manVec3.create(NULL, matrix->data[0].x, matrix->data[1].x, matrix->data[2].x);
    Vec3 row1 = manVec3.create(NULL, matrix->data[0].y, matrix->data[1].y, matrix->data[2].y);
    Vec3 row2 = manVec3.create(NULL, matrix->data[0].z, matrix->data[1].z, matrix->data[2].z);

    scalar x = manVec3.dot(&row0, col);
    scalar y = manVec3.dot(&row1, col);
    scalar z = manVec3.dot(&row2, col);

    return (manVec3.create(NULL, x, y, z));
}

/**
 *  Return a Vec3 equal to the result of multiplying the given Mat3 and the given Vec3 row vector.
 *  (ie. row * matrix).
 *
 *  @param  row     const pointer to const Vec3, row-vector.
 *  @param  matrix  const pointer to const Mat3.
 *  @return         Vec3 equal to result of row * matrix.
 */
Vec3 mulVec3Mat3(const Vec3 *const row, const Mat3 *const matrix) {
    Vec3 col0 = manVec3.create(NULL, matrix->data[0].x, matrix->data[1].x, matrix->data[2].x);
    Vec3 col1 = manVec3.create(NULL, matrix->data[0].y, matrix->data[1].y, matrix->data[2].y);
    Vec3 col2 = manVec3.create(NULL, matrix->data[0].z, matrix->data[1].z, matrix->data[2].z);

    scalar x = manVec3.dot(&col0, row);
    scalar y = manVec3.dot(&col1, row);
    scalar z = manVec3.dot(&col2, row);

    return (manVec3.create(NULL, x, y, z));
}
