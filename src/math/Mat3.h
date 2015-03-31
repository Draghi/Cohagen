#ifndef MATH_MAT3_H
#define MATH_MAT3_H

#include "Precision.h"
#include "Vec3.h"

typedef struct Mat3_s {
    Vec3 data[3];
} Mat3;

typedef struct Mat3Manager_s {
	/**
     *  Returns a Mat3 object constructed from the given elements.
     *  Optionally, a pointer to a Mat3 may be passed in, if not NULL, the
     *  given Mat3 will be filled with the constructed data.
     *  Elements are indexed in the following way: el<col, row>.
     *  Note column-major order.
     *
     *  @param  mat     pointer to const Mat3 to fill up with data, or NULL.
     *  @param  el00    scalar, col 0, row 0.
     *  @param  el01    scalar, col 0, row 1.
     *  @param  el02    scalar, col 0, row 2.
     *  @param  el10    scalar, col 1, row 0.
     *  @param  el11    scalar, col 1, row 1.
     *  @param  el12    scalar, col 1, row 2.
     *  @param  el20    scalar, col 2, row 0.
     *  @param  el21    scalar, col 2, row 1.
     *  @param  el22    scalar, col 2, row 2.
     *  @return         Mat3 constructed from given elements.
     */
     Mat3 (*create)( Mat3 *const mat,
					scalar el00, scalar el10, scalar el20,
                    scalar el01, scalar el11, scalar el21,
                    scalar el02, scalar el12, scalar el22);
    /**
     *  Returns a Mat3 with the "leading" value along the main diagonal
     *  and 0 everywhere else.
     *  Optionally, a pointer to a Mat3 may be passed in, if not NULL, the
     *  given Mat3 will be filled with the constructed data.
     *
     *  @param  mat         const pointer to Mat3 to fill up with data, or NULL.
     *  @param  leading     scalar, value to place along main diagnoal.
     *  @return             Mat3 with given value along leading diagonal and zero elsewhere.
     */
	Mat3 (*createLeading)(Mat3 *const mat, scalar leading);

    /**
     *  Returns a Mat3 constructed with a series of column vectors.
     *  Optionally, a pointer to a Mat3 may be passed in, if not NULL, the
     *  given Mat3 will be filled with the constructed data.
     *
     *  @param  mat const pointer to Mat3 to fill up with data, or NULL.
     *  @param  v0  const pointer to const Vec4, column 0.
     *  @param  v1  const pointer to const Vec4, column 1.
     *  @param  v2  const pointer to const Vec4, column 2.
     *  @return     Mat3 constructed from given column-vectors.
     */
	Mat3 (*createFromVec3)(Mat3 *const mat, const Vec3 *const v0, const Vec3 *const v1, const Vec3 *const v2);

    /**
     *  Returns a Mat3 constructed from another Mat3 (copy constructor).
     *  Optionally, a pointer to a Mat3 may be passed in, if not NULL, the
     *  given Mat3 will be filled with the constructed data.
     *
     *  @param  mat     const pointer to Mat3 to fill up with data, or NULL.
     *  @param  matrix  const pointer to const Mat4 to clone.
     *  @return         Mat3 constructed from given Mat3.
     */
	Mat3 (*createFromMat3)(Mat3 *const mat, const Mat3 *const mat3);

	/**
	 *  Return a Mat3 that is the result of the component-wise sum of
	 *  mat1 and mat2. (ie. mat1 + mat2).
	 *
	 *  @param mat1     const pointer to const matrix 1.
	 *  @param mat2     const pointer to const matrix 2.
	 *  @return         Mat3 equal to mat1 + mat2 (component-wise sum).
	 */
	Mat3 (*sum)(const Mat3 *const mat1, const Mat3 *const mat2);

	/**
	 *  Return a Mat3 that is the result of the component-wise subtraction of
	 *  mat2 from mat1. (ie. mat1 - mat2).
	 *
	 *  @param mat1     const pointer to const matrix 1.
	 *  @param mat2     const pointer to const matrix 2.
	 *  @return         Mat3 equal to mat1 - mat2 (component-wise subtraction).
	 */
	Mat3 (*sub)(const Mat3 *const mat1, const Mat3 *const mat2);

	/**
	 *  Return a Mat3 equal to the result of the matrix multiplication operation
	 *  between mat1 and mat2. (ie. mat1 * mat2).
	 *
	 *  @param mat1     const pointer to const matrix 1.
	 *  @param mat2     const pointer to const matrix 2.
	 *  @return         Mat3 equal to the result of the matrix multiplication operation between
	 *                  mat1 and mat2.
	 */
	Mat3 (*mul)(const Mat3 *const mat1, const Mat3 *const mat2);

	/**
	 *  Return a Vec3 equal to the result of multiplying the given Mat3 and the given Vec3 column vector.
	 *  (ie. matrix * col).
	 *
	 *  @param  matrix  const pointer to const Mat3.
	 *  @param  col     const pointer to const Vec3, column-vector.
	 *  @return         Vec3 equal to result of matrix * col.
	 */
	Mat3 (*postMulVec3)(const Mat3 *const matrix, const Vec3 *const col);

	/**
	 *  Return a Vec3 equal to the result of multiplying the given Mat3 and the given Vec3 row vector.
	 *  (ie. row * matrix).
	 *
	 *  @param  row     const pointer to const Vec3, row-vector.
	 *  @param  matrix  const pointer to const Mat3.
	 *  @return         Vec3 equal to result of row * matrix.
	 */
	Mat3 (*preMulVec3)(const Vec3 *const row, const Mat3 *const matrix);

    /**
     *  Multiply every element of the given Mat3 by a constant factor
     *  and return the result.
     *
     *  @param  matrix  const pointer to const Mat3, matrix to multiply.
     *  @param  factor  scalar, factor to multiply each element by.
     *  @return         Mat3, given Mat3 with each element multiplied by given factor.
     */
	Mat3 (*postMulScalar)(const Mat3 *const matrix, scalar factor);

	/**
     *  Multiply every element of the given Mat3 by a constant factor
     *  and return the result.
     *
     *  @param  matrix  const pointer to const Mat3, matrix to multiply.
     *  @param  factor  scalar, factor to multiply each element by.
     *  @return         Mat3, given Mat3 with each element multiplied by given factor.
     */
	Mat3 (*preMulScalar)(scalar factor, const Mat3 *const matrix);

} Mat3Manager;

Mat3 createMat3(        scalar el00, scalar el10, scalar el20,
                        scalar el01, scalar el11, scalar el21,
                        scalar el02, scalar el12, scalar el22);
Mat3 createMat3Leading(scalar leading);
Mat3 createMat3Vec3(const Vec3 *const v0, const Vec3 *const v1, const Vec3 *const v2);
Mat3 createMat3Mat3(const Mat3 *const matrix);

Mat3 sumMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2);
Mat3 subMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2);
Mat3 mulMat3Mat3(const Mat3 *const mat1, const Mat3 *const mat2);
Vec3 mulMat3Vec3(const Mat3 *const matrix, const Vec3 *const col);
Vec3 mulVec3Mat3(const Vec3 *const row, const Mat3 *const matrix);
Mat3 inverseMat3(const Mat3 *const matrix);

#endif