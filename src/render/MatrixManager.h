#ifndef COH_MATRIXMANAGER_H
#define COH_MATRIXMANAGER_H

#include <stdbool.h>
#include <stdint.h>

#include "math/Precision.h"
#include "math/Vec3.h"
#include "math/Mat4.h"
#include "util/Stack.h"

extern const uint32_t MATRIX_MODE_PROJECTION;
extern const uint32_t MATRIX_MODE_VIEW;
extern const uint32_t MATRIX_MODE_MODEL;
extern const uint32_t MATRIX_MODE_COUNT;

/**
 *	Matrix Manager class.
 *
 * 	Used to provide matrix stack behaviour for transformations.
 */
typedef struct MatrixManager_s {
	/** The stacks where matricies are stored */
	Stack** stacks;

	/** The current stack mode the manager is in */
	uint32_t matMode;

	/** Used so we don't initialize twice. */
	bool initialized;
} MatrixManager;

/**
 *	Manager class for Matrix Manager objects.
 */
typedef struct MatrixManagerManager_s {
	/**
	 * Creates a new matrix manager.
	 */
	MatrixManager*(* new)();

	/**
	 * Changes which matrix stack to perform operations on.
	 * @param mode - The mode to change to (MATRIX_MODE_PROJECTION eg.)
	 * @return Whether the mode was changed or not.
	 */
	bool (* setMode)(MatrixManager*, uint32_t);

	/**
	 * Returns the next element in the current stack, without removing it.
	 * @return The next element in the current stack, without removing it. NULL if the stack is empty.
	 */
	Mat4*(* peek)(MatrixManager*);

	/**
	 * Returns the next element in the given stack, without removing it.
	 * @param mode The stack to peek at.
	 * @return The next element in the given stack, without removing it. NULL if the stack is empty or an invalid mode is given.
	 */
	Mat4*(* peekStack)(MatrixManager*, uint32_t);

	/**
	 * Copies the next matrix in the stack and pushes it onto the stack.
	 */
	void (* push)(MatrixManager*);

	/**
	 * Pushes the given matrix onto the current stack.
	 * The matrix can be on either the heap or stack and it will be copied to the stack.
	 * @param mat The matrix to push onto the current stack.
	 */
	void (* pushMat4)(MatrixManager*, Mat4*);

	/**
	 * Pushes a new identity matrix onto the stack.
	 */
	void (* pushIdentity)(MatrixManager*);

	/**
	 * Pushes a new perspective matrix onto the current stack.
	 * @param fov The horizontal fov in radians.
	 * @param aspect The aspect ratio of the screen.
	 * @param near The closest clipping plane's distance (Shouldn't be 0)
	 * @param far The farthest clipping plane's distance.
	 */
	void (* pushPerspective)(MatrixManager*, scalar, scalar, scalar, scalar);

	/**
	 * Removes the next matrix in the current stack and returns it.
	 * @return The old next matrix in the current stack.
	 */
	Mat4*(* pop)(MatrixManager*);

	/**
	 * Rotates the next element in current stack by the given angle around the given axis.
	 * @param angle The angle to rotate by in radians.
	 * @param axis The axis to rotate around (should be normalized)
	 */
	void (* rotate)(MatrixManager*, scalar, Vec3);

	/**
	 * Translates the next element in the current stack by the given vector.
	 * @param translation The vector containing how far to translate the matrix by along each axis.
	 */
	void (* translate)(MatrixManager*, Vec3);

	/**
	 * Scales the next element in the current stack by the given vector.
	 * @param scale The vector containing how much each axis should be scaled by.
	 */
	void (* scale)(MatrixManager*, Vec3);

	/**
	 * Multiplies the next matrix in the current stack by the given matrix.
	 * Useful for quarternion rotations.
	 * @param matrix The matrix to multiply with.
	 */
	void (* mult)(MatrixManager*, Mat4*);

	/**
	 * Clear a matrix manager.
	 */
	void(* clear)(MatrixManager* manager);

	/**
	 * Frees a matrix manager.
	 */
	void(* delete)(MatrixManager* manager);
} MatrixManagerManager;

extern const MatrixManagerManager manMatMan;


#endif
