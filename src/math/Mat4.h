#ifndef MATH_MAT4_H
#define MATH_MAT4_H

#include "Vec4.h"

typedef struct Mat4_s {
    Vec4 data[4];
} Mat4;

typedef struct Mat4Manager_s {
    /**
     *  Returns a Mat4 object constructed from the given elements.
     *  Optionally, a pointer to a Mat4 may be passed in, if not NULL, the
     *  given Mat4 will be filled with the constructed data.
     *  Elements are indexed in the following way: el<col, row>.
     *  Note column-major order.
     *
     *  @param  mat     pointer to const Mat4 to fill up with data, or NULL.
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
    Mat4 (*create)( Mat4 *const mat,
                    scalar el00, scalar el10, scalar el20, scalar el30, 
                    scalar el01, scalar el11, scalar el21, scalar el31,
                    scalar el02, scalar el12, scalar el22, scalar el32,
                    scalar el03, scalar el13, scalar el23, scalar el33);

    /**
     *  Returns a Mat4 with the "leading" value along the main diagonal
     *  and 0 everywhere else.
     *  Optionally, a pointer to a Mat4 may be passed in, if not NULL, the
     *  given Mat4 will be filled with the constructed data.
     *
     *  @param  mat         const pointer to Mat4 to fill up with data, or NULL.
     *  @param  leading     scalar, value to place along main diagnoal.
     *  @return             Mat4 with given value along leading diagonal and zero elsewhere.
     */
    Mat4 (*createLeading)(Mat4 *const mat, scalar leading);

    /**
     *  Returns a Mat4 constructed with a series of column vectors.
     *  Optionally, a pointer to a Mat4 may be passed in, if not NULL, the
     *  given Mat4 will be filled with the constructed data.
     *
     *  @param  mat const pointer to Mat4 to fill up with data, or NULL.
     *  @param  v0  const pointer to const Vec4, column 0.
     *  @param  v1  const pointer to const Vec4, column 1.
     *  @param  v2  const pointer to const Vec4, column 2.
     *  @param  v3  const pointer to const Vec4, column 3.
     *  @return     Mat4 constructed from given column-vectors.
     */
    Mat4 (*createFromVec4)(Mat4 *const mat, const Vec4 *const v0, const Vec4 *const v1, const Vec4 *const v2, const Vec4 *const v3);

    /**
     *  Returns a Mat4 constructed from another Mat4 (copy constructor).
     *  Optionally, a pointer to a Mat4 may be passed in, if not NULL, the
     *  given Mat4 will be filled with the constructed data.
     *
     *  @param  mat     const pointer to Mat4 to fill up with data, or NULL.
     *  @param  matrix  const pointer to const Mat4 to clone.
     *  @return         Mat4 constructed from given Mat4.
     */
    Mat4 (*createFromMat4)(Mat4 *const mat, const Mat4 *const matrix);

    /**
     *  Returns a Mat4, the result of the component-wise sum of mat1 + mat2.
     *
     *  @param  mat1    const pointer to const Mat4, operand 1.
     *  @param  mat2    const pointer to const Mat4, operand 2.
     *  @return         Mat4, result of component-wise sum of mat1 + mat2.
     */
    Mat4 (*sum)(const Mat4 *const mat1, const Mat4 *const mat2);

    /**
     *  Returns a Mat4, the result of the component-wise subtraction of mat1 - mat2.
     *
     *  @param  mat1    const pointer to const Mat4, operand 1.
     *  @param  mat2    const pointer to const Mat4, operand 2.
     *  @return         Mat4, result of component-wise subtraction of mat1 - mat2.
     */
    Mat4 (*sub)(const Mat4 *const mat1, const Mat4 *const mat2);

    /**
     *  Multiply every element of the given Mat4 by a constant factor
     *  and return the result.
     *
     *  @param  matrix  const pointer to const Mat4, matrix to multiply.
     *  @param  factor  scalar, factor to multiply each element by.
     *  @return         Mat4, given Mat4 with each element multiplied by given factor.
     */
    Mat4 (*postMulScalar)(const Mat4 *const matrix, scalar factor);

    /**
     *  Multiply every element of the given Mat4 by a constant factor
     *  and return the result.
     *
     *  @param  factor  scalar, factor to multiply each element by.
     *  @param  matrix  const pointer to const Mat4, matrix to multiply.
     *  @return         Mat4, given Mat4 with each element multiplied by given factor.
     */
    Mat4 (*preMulScalar)(scalar factor, const Mat4 *const matrix);

    /** 
     *  Returns a Mat4, result of matrix multiplication between operands given.
     *  (ie. mat1 * mat2, in that order).
     *
     *  @param  mat1    const pointer to const Mat4, operand 1.
     *  @param  mat2    const pointer to const Mat4, operand 2.
     *  @return         Mat4, result of matrix multiplication (mat1 * mat2).
     */
    Mat4 (*mul)(const Mat4 *const mat1, const Mat4 *const mat2);

    /**
     *  Returns a Vec4, result of multiplication between a Mat4 and Vec4 column.
     *  (ie. matrix * col)
     *
     *  @param  col     const pointer to const Vec4, column vector.
     *  @param  matrix  const pointer to const Mat4, matrix.
     *  @return         Vec4, resulting column vector.
     */
    Vec4 (*postMulVec4)(const Mat4 *const matrix, const Vec4 *const col);

    /**
     *  Returns a Vec4, result of multiplication between a Mat4 and Vec4 row.
     *  (ie. row * matrix)
     *
     *  @param  row     const pointer to const Vec4, row vector.
     *  @param  matrix  const pointer to const Mat4, matrix.
     *  @return         Vec4, resulting column vector.
     */
    Vec4 (*preMulVec4)(const Vec4 *const row, const Mat4 *const matrix);
    
    /**
     *  Returns the inverse of the given Mat4.
     *
     *  @param  matrix  const pointer to const Mat4, matrix to find the inverse of.
     *  @return         Mat4, inverse of given Mat4.
     */
     Mat4 (*inverse)(const Mat4 *const matrix);
} Mat4Manager;

// Mat4 createMat4(        scalar el00, scalar el10, scalar el20, scalar el30, 
//                         scalar el01, scalar el11, scalar el21, scalar el31,
//                         scalar el02, scalar el12, scalar el22, scalar el32,
//                         scalar el03, scalar el13, scalar el23, scalar el33);
// Mat4 createMat4Leading(scalar leading);
// Mat4 createMat4Vec4(const Vec4 *const v0, const Vec4 *const v1, const Vec4 *const v2, const Vec4 *const v3);
// Mat4 createMat4Mat4(const Mat4 *const matrix);

// Mat4 sumMat4Mat4(const Mat4 *const mat1, const Mat4 *const mat2);
// Mat4 subMat4Mat4(const Mat4 *const mat1, const Mat4 *const mat2);
// Mat4 mulMat4Scalar(const Mat4 *const matrix, scalar factor);
// Mat4 mulMat4Mat4(const Mat4 *const mat1, const Mat4 *const mat2);
// Vec4 mulMat4Vec4(const Mat4 *const matrix, const Vec4 *const row);
// Vec4 mulVec4Mat4(const Vec4 *const col, const Mat4 *const matrix);
// Mat4 inverseMat4(const Mat4 *const matrix);

extern const Mat4Manager manMat4;
#endif