#include "Mat3.h"

/**
 *  Return a Mat3 made up of the given scalars.
 *
 *  Note column-major construction:
 *      colRow colRow colRow
 *      colRow colRow colRow
 *      colRow colRow colRow
 *
 *  @param  el00    scalar, col 0 row 0
 *  @param  el01    scalar, col 0 row 1
 *  @param  el02    scalar, col 0 row 2
 *  @param  el10    scalar, col 1 row 0
 *  @param  el11    scalar, col 1 row 1
 *  @param  el12    scalar, col 1 row 2
 *  @param  el20    scalar, col 2 row 0
 *  @param  el21    scalar, col 2 row 1
 *  @param  el22    scalar, col 2 row 2
 *  @return         Mat3 made up of the given scalars.
 */
Mat3 createMat3(    scalar el00, scalar el10, scalar el20,
                    scalar el01, scalar el11, scalar el21,
                    scalar el02, scalar el12, scalar el22) {
    Mat3 matrix;

    matrix.data[0] = createVec3(el00, el01, el02);
    matrix.data[1] = createVec3(el10, el11, el12);
    matrix.data[2] = createVec3(el20, el21, el22);

    return matrix;
}

/**
 *  Return a Mat3 with the given value along the leading diagonal
 *  and 0 everywhere else.
 *
 *  @param leading  scalar, the leading value to use.
 *  @return         Mat3 with "leading" value along main diagonal and 0 everywhere else.
 */
Mat3 createMat3Leading(scalar leading) {
    Mat3 matrix;

    matrix.data[0] = createVec3(leading, 0.0f, 0.0f);
    matrix.data[1] = createVec3(0.0f, leading, 0.0f);
    matrix.data[2] = createVec3(0.0f, 0.0f, leading);

    return matrix;
}

/**
 *  Return a Mat3 constructed from the given Vec3s.
 *
 *  @param v0   const pointer to const Vec3, column 0.
 *  @param v1   const pointer to const Vec3, column 1.
 *  @param v2   const pointer to const Vec3, column 2.
 *  @return     Mat3 made up of given Vec3s.
 */
Mat3 createMat3Vec3(const Vec3 *const v0, const Vec3 *const v1, const Vec3 *const v2) {
    Mat3 matrix;

    matrix.data[0] = *v0;
    matrix.data[1] = *v1;
    matrix.data[2] = *v2;

    return matrix;
}

/**
 *  Return a Mat3 that is a clone of the given Mat3.
 *
 *  @param matrix   const pointer to const Mat3 to clone.
 *  @return         Mat3 clone.
 */
Mat3 createMat3Mat3(const Mat3 *const matrix) {
    return (
        createMat3Vec3(
            &(matrix->data[0]),
            &(matrix->data[1]),
            &(matrix->data[2])
        )
    );
}

/**
 *  Return a Mat3 that is the result of the component-wise sum of
 *  mat1 and mat2. (ie. mat1 + mat2).
 *
 *  @param mat1     const pointer to const matrix 1.
 *  @param mat2     const pointer to const matrix 2.
 *  @return         Mat3 equal to mat1 + mat2 (component-wise sum).
 */
Mat3 sumMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2) {
    Vec3 v0 = sumVec3Vec3(&(mat1->data[0]), &(mat2->data[0]));
    Vec3 v1 = sumVec3Vec3(&(mat1->data[1]), &(mat2->data[1]));
    Vec3 v2 = sumVec3Vec3(&(mat1->data[2]), &(mat2->data[2]));

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
    Vec3 v0 = subVec3Vec3(&(mat1->data[0]), &(mat2->data[0]));
    Vec3 v1 = subVec3Vec3(&(mat1->data[1]), &(mat2->data[1]));
    Vec3 v2 = subVec3Vec3(&(mat1->data[2]), &(mat2->data[2]));

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
    Vec3 row0 = createVec3(mat1->data[0].x, mat1->data[1].x, mat1->data[2].x);
    Vec3 row1 = createVec3(mat1->data[0].y, mat1->data[1].y, mat1->data[2].y);
    Vec3 row2 = createVec3(mat1->data[0].z, mat1->data[1].z, mat1->data[2].z);
    Vec3 col0 = mat2->data[0];
    Vec3 col1 = mat2->data[1];
    Vec3 col2 = mat2->data[2];

    float x0 = dotVec3(&row0, &col0);
    float y0 = dotVec3(&row1, &col0);
    float z0 = dotVec3(&row2, &col0);
    float x1 = dotVec3(&row0, &col1);
    float y1 = dotVec3(&row1, &col1);
    float z1 = dotVec3(&row2, &col1);
    float x2 = dotVec3(&row0, &col2);
    float y2 = dotVec3(&row1, &col2);
    float z2 = dotVec3(&row2, &col2);

    Vec3 v0 = createVec3(x0, y0, z0);
    Vec3 v1 = createVec3(x1, y1, z1);
    Vec3 v2 = createVec3(x2, y2, z2);
    
    return (createMat3Vec3(&v0, &v1, &v2));
}

/**
 *  Return a Vec3 equal to the result of multiplying the given Mat3 and the given Vec3.
 *  (ie. matrix * vec).
 *
 *  @param  matrix  const pointer to const Mat3.
 *  @param  vec     const pointer to const Vec3.
 *  @return         Vec3 equal to result of matrix * vec.
 */
Vec3 mulMat3Vec3(const Mat3 *const matrix, const Vec3 *const vec) {
    Vec3 row0 = createVec3(matrix->data[0].x, matrix->data[1].x, matrix->data[2].x);
    Vec3 row1 = createVec3(matrix->data[0].y, matrix->data[1].y, matrix->data[2].y);
    Vec3 row2 = createVec3(matrix->data[0].z, matrix->data[1].z, matrix->data[2].z);

    scalar x = dotVec3(&row0, vec);
    scalar y = dotVec3(&row1, vec);
    scalar z = dotVec3(&row2, vec);

    return (createVec3(x, y, z));
}