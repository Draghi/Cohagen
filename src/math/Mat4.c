#include "Mat4.h"

/**
 *  Returns a Mat4 object constructed from the given elements.
 *  Elements are indexed in the following way: el<col, row>.
 *  Note column-major order.
 *
 *  @param  el00    scalar, col 0, row 0.
 *  @param  el01    scalar, col 0, row 1.
 *  @param  el02    scalar, col 0, row 2.
 *  @param  el03    scalar, col 0, row 3.
 *  @param  el10    scalar, col 1, row 0.
 *  @param  el11    scalar, col 1, row 1.
 *  @param  el12    scalar, col 1, row 2.
 *  @param  el13    scalar, col 1, row 3.
 *  @param  el20    scalar, col 2, row 0.
 *  @param  el21    scalar, col 2, row 1.
 *  @param  el22    scalar, col 2, row 2.
 *  @param  el23    scalar, col 2, row 3.
 *  @param  el30    scalar, col 3, row 0.
 *  @param  el31    scalar, col 3, row 1.
 *  @param  el32    scalar, col 3, row 2.
 *  @param  el33    scalar, col 3, row 3.
 *  @return         Mat4 constructed from given elements.
 */
Mat4 createMat4(        scalar el00, scalar el10, scalar el20, scalar el30, 
                        scalar el01, scalar el11, scalar el21, scalar el31,
                        scalar el02, scalar el12, scalar el22, scalar el32,
                        scalar el03, scalar el13, scalar el23, scalar el33) {
    Mat4 matrix;

    matrix.data[0] = createVec4(el00, el01, el02, el03);
    matrix.data[1] = createVec4(el10, el11, el12, el13);
    matrix.data[2] = createVec4(el20, el21, el22, el23);
    matrix.data[3] = createVec4(el30, el31, el32, el33);

    return matrix;
}

/**
 *  Returns a Mat4 with the "leading" value along the main diagonal
 *  and 0 everywhere else.
 *
 *  @param  leading     scalar, value to place along main diagnoal.
 *  @return             Mat4 with given value along leading diagonal and zero elsewhere.
 */
Mat4 createMat4Leading(scalar leading) {
    Mat4 matrix;

    matrix.data[0] = createVec4(leading, 0.0f, 0.0f, 0.0f);
    matrix.data[1] = createVec4(0.0f, leading, 0.0f, 0.0f);
    matrix.data[2] = createVec4(0.0f, 0.0f, leading, 0.0f);
    matrix.data[3] = createVec4(0.0f, 0.0f, 0.0f, leading);

    return matrix;
}

/**
 *  Returns a Mat4 constructed with a series of column vectors.
 *
 *  @param  v0  const pointer to const Vec4, column 0.
 *  @param  v1  const pointer to const Vec4, column 1.
 *  @param  v2  const pointer to const Vec4, column 2.
 *  @param  v3  const pointer to const Vec4, column 3.
 *  @return     Mat4 constructed from given column-vectors.
 */
Mat4 createMat4Vec4(const Vec4 *const v0, const Vec4 *const v1, const Vec4 *const v2, const Vec4 *const v3) {
    Mat4 matrix;

    matrix.data[0] = *v0;
    matrix.data[1] = *v1;
    matrix.data[2] = *v2;
    matrix.data[3] = *v3;

    return matrix;
}

/**
 *  Returns a Mat4 constructed from another Mat4 (copy constructor).
 *
 *  @param  matrix  const pointer to const Mat4 to clone.
 *  @return         Mat4 constructed from given Mat4.
 */
Mat4 createMat4Mat4(const Mat4 *const matrix) {
    return (
        createMat4Vec4(
            &(matrix->data[0]),
            &(matrix->data[1]),
            &(matrix->data[2]),
            &(matrix->data[3])
        )
    );
}

/**
 *  Returns a Mat4, the result of the component-wise sum of mat1 + mat2.
 *
 *  @param  mat1    const pointer to const Mat4, operand 1.
 *  @param  mat2    const pointer to const Mat4, operand 2.
 *  @return         Mat4, result of component-wise sum of mat1 + mat2.
 */
Mat4 sumMat4Mat4(const Mat4 *const mat1, const Mat4 *const mat2) {
    Vec4 v0 = sumVec4Vec4(&(mat1->data[0]), &(mat2->data[0]));
    Vec4 v1 = sumVec4Vec4(&(mat1->data[1]), &(mat2->data[1]));
    Vec4 v2 = sumVec4Vec4(&(mat1->data[2]), &(mat2->data[2]));
    Vec4 v3 = sumVec4Vec4(&(mat1->data[3]), &(mat2->data[3]));

    return (
        createMat4Vec4(
            &v0,
            &v1,
            &v2,
            &v3
        )
    );
}

/**
 *  Returns a Mat4, the result of the component-wise subtraction of mat1 - mat2.
 *
 *  @param  mat1    const pointer to const Mat4, operand 1.
 *  @param  mat2    const pointer to const Mat4, operand 2.
 *  @return         Mat4, result of component-wise subtraction of mat1 - mat2.
 */
Mat4 subMat4Mat4(const Mat4 *const mat1, const Mat4 *const mat2) {
    Vec4 v0 = subVec4Vec4(&(mat1->data[0]), &(mat2->data[0]));
    Vec4 v1 = subVec4Vec4(&(mat1->data[1]), &(mat2->data[1]));
    Vec4 v2 = subVec4Vec4(&(mat1->data[2]), &(mat2->data[2]));
    Vec4 v3 = subVec4Vec4(&(mat1->data[3]), &(mat2->data[3]));

    return (
        createMat4Vec4(
            &v0,
            &v1,
            &v2,
            &v3
        )
    );
}

/** 
 *  Returns a Mat4, result of matrix multiplication between operands given.
 *  (ie. mat1 * mat2, in that order).
 *
 *  @param  mat1    const pointer to const Mat4, operand 1.
 *  @param  mat2    const pointer to const Mat4, operand 2.
 *  @return         Mat4, result of matrix multiplication (mat1 * mat2).
 */
Mat4 mulMat4Mat4(const Mat4 *const mat1, const Mat4 *const mat2) {
    Vec4 row0 = createVec4(mat1->data[0].x, mat1->data[1].x, mat1->data[2].x, mat1->data[3].x);
    Vec4 row1 = createVec4(mat1->data[0].y, mat1->data[1].y, mat1->data[2].y, mat1->data[3].y);
    Vec4 row2 = createVec4(mat1->data[0].z, mat1->data[1].z, mat1->data[2].z, mat1->data[3].z);
    Vec4 row3 = createVec4(mat1->data[0].w, mat1->data[1].w, mat1->data[2].w, mat1->data[3].w);

    Vec4 col0 = mat2->data[0];
    Vec4 col1 = mat2->data[1];
    Vec4 col2 = mat2->data[2];
    Vec4 col3 = mat2->data[3];

    scalar x0 = dotVec4(&row0, &col0);
    scalar y0 = dotVec4(&row1, &col0);
    scalar z0 = dotVec4(&row2, &col0);
    scalar w0 = dotVec4(&row3, &col0);
    scalar x1 = dotVec4(&row0, &col1);
    scalar y1 = dotVec4(&row1, &col1);
    scalar z1 = dotVec4(&row2, &col1);
    scalar w1 = dotVec4(&row3, &col1);
    scalar x2 = dotVec4(&row0, &col2);
    scalar y2 = dotVec4(&row1, &col2);
    scalar z2 = dotVec4(&row2, &col2);
    scalar w2 = dotVec4(&row3, &col2);
    scalar x3 = dotVec4(&row0, &col3);
    scalar y3 = dotVec4(&row1, &col3);
    scalar z3 = dotVec4(&row2, &col3);
    scalar w3 = dotVec4(&row3, &col3);

    Vec4 v0 = createVec4(x0, y0, z0, w0);
    Vec4 v1 = createVec4(x1, y1, z1, w1);
    Vec4 v2 = createVec4(x2, y2, z2, w2);
    Vec4 v3 = createVec4(x3, y3, z3, w3);

    return (
        createMat4Vec4(
            &v0,
            &v1,
            &v2,
            &v3
        )
    );
}

/**
 *  Returns a Vec4, result of multiplication between a Mat4 and Vec4 column.
 *  (ie. matrix * col)
 *
 *  @param  col     const pointer to const Vec4, column vector.
 *  @param  matrix  const pointer to const Mat4, matrix.
 *  @return         Vec4, resulting column vector.
 */
Vec4 mulMat4Vec4(const Mat4 *const matrix, const Vec4 *const col) {
    Vec4 row0 = createVec4(matrix->data[0].x, matrix->data[1].x, matrix->data[2].x, matrix->data[3].x);
    Vec4 row1 = createVec4(matrix->data[0].y, matrix->data[1].y, matrix->data[2].y, matrix->data[3].y);
    Vec4 row2 = createVec4(matrix->data[0].z, matrix->data[1].z, matrix->data[2].z, matrix->data[3].z);
    Vec4 row3 = createVec4(matrix->data[0].w, matrix->data[1].w, matrix->data[2].w, matrix->data[3].w);

    return (
        createVec4(
            dotVec4(&row0, col),
            dotVec4(&row1, col),
            dotVec4(&row2, col),
            dotVec4(&row3, col)
        )
    );
}

/**
 *  Returns a Vec4, result of multiplication between a Mat4 and Vec4 row.
 *  (ie. row * matrix)
 *
 *  @param  row     const pointer to const Vec4, row vector.
 *  @param  matrix  const pointer to const Mat4, matrix.
 *  @return         Vec4, resulting column vector.
 */
Vec4 mulVec4Mat4(const Vec4 *const row, const Mat4 *const matrix) {
    Vec4 col0 = createVec4(matrix->data[0].x, matrix->data[0].y, matrix->data[0].z, matrix->data[0].w);
    Vec4 col1 = createVec4(matrix->data[1].x, matrix->data[1].y, matrix->data[1].z, matrix->data[1].w);
    Vec4 col2 = createVec4(matrix->data[2].x, matrix->data[2].y, matrix->data[2].z, matrix->data[2].w);
    Vec4 col3 = createVec4(matrix->data[3].x, matrix->data[3].y, matrix->data[3].z, matrix->data[3].w);

    return (
        createVec4(
            dotVec4(row, &col0),
            dotVec4(row, &col1),
            dotVec4(row, &col2),
            dotVec4(row, &col3)
        )
    );
}

/**
 *  Returns the inverse of the given Mat4.
 *
 *  @param  matrix  const pointer to const Mat4, matrix to find the inverse of.
 *  @return         Mat4, inverse of given Mat4.
 */
// Mat4 inverseMat4(const Mat4 *const matrix);

