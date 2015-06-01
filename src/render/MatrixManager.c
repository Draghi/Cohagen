#include "MatrixManager.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#include "math/Precision.h"
#include "math/Vec3.h"
#include "math/Mat4.h"

/** The Projection Matrix Stack **/
const uint32_t MATRIX_MODE_PROJECTION = 0x0000;

/** The View Matrix Stack **/
const uint32_t MATRIX_MODE_VIEW  = 0x0001;

/** The Model Matrix Stack **/
const uint32_t MATRIX_MODE_MODEL = 0x0002;

/** The Number of Matrix manager->stacks **/
const uint32_t MATRIX_MODE_COUNT = 3;

/**
 * Should be called at the beginning to prepare the matrix manager.
 */
static MatrixManager* new() {
	MatrixManager* manager = malloc(sizeof(MatrixManager));
	manager->stacks = malloc(sizeof(Stack*)*MATRIX_MODE_COUNT);
	for(int i = 0; i < MATRIX_MODE_COUNT; i++) {
		manager->stacks[i] = manStack.new();
	}

	manager->matMode = 0;
	manager->initialized = true;

	return manager;
}

/**
 * Changes which matrix stack to perform operations on.
 * @param mode - The mode to change to (MATRIX_MODE_PROJECTION eg.)
 * @return Whether the mode was changed or not.
 */
static bool setMode(MatrixManager* manager, uint32_t mode) {
	if (mode<MATRIX_MODE_COUNT) {
		manager->matMode = mode;
		return true;
	} else {
		return false;
	}
}

/**
 * Returns the next element in the current stack, without removing it.
 * @return The next element in the current stack, without removing it. NULL if the stack is empty.
 */
static Mat4* peek(MatrixManager* manager) {
	Stack* stack = manager->stacks[manager->matMode];
	if (stack->elementSize>0)
		return (Mat4*) manStack.peek(stack);
	else
		return NULL;
}

/**
 * Returns the next element in the given stack, without removing it.
 * @param mode The stack to peek at.
 * @return The next element in the given stack, without removing it. NULL if the stack is empty or an invalid mode is given.
 */
static Mat4* peekStack(MatrixManager* manager, uint32_t mode) {
	if (mode<MATRIX_MODE_COUNT) {
		Stack* stack = manager->stacks[mode];
		if (stack->elementSize>0)
			return (Mat4*) manStack.peek(stack);
	}
	return NULL;
}

/**
 * Pushes the given matrix onto the current stack.
 * The matrix can be on either the heap or stack and it will be copied to the stack.
 * @param mat The matrix to push onto the current stack.
 */
static void pushMat4(MatrixManager* manager, Mat4* mat) {
	if (mat!=NULL) {
		Stack* stack = manager->stacks[manager->matMode];
		Mat4* matClone = malloc(sizeof(Mat4));
		manMat4.createFromMat4(matClone, mat);
		manStack.push(stack, matClone);
	}
}

/**
 * Copies the next matrix in the stack and pushes it onto the stack.
 */
static void push(MatrixManager* manager) {
	pushMat4(manager, peek(manager));
}

/**
 * Pushes a new identity matrix onto the stack.
 */
static void pushIdentity(MatrixManager* manager) {
	Stack* stack = manager->stacks[manager->matMode];
	Mat4* mat = malloc(sizeof(Mat4));
	manMat4.createLeading(mat, 1);
	manStack.push(stack, mat);
}

/**
 * Pushes a new perspective matrix onto the current stack.
 * @param fov The horizontal fov in radians.
 * @param aspect The aspect ratio of the screen.
 * @param near The closest clipping plane's distance (Shouldn't be 0)
 * @param far The farthest clipping plane's distance.
 */
static void pushPerspective(MatrixManager* manager, scalar fov, scalar aspect, scalar near, scalar far) {
	Stack* stack = manager->stacks[manager->matMode];

	Mat4* mat = malloc(sizeof(Mat4));
	scalar x0, y1, z2, z3, w2;

	if (far == 0) {
		//Projection matrix without far clipping plane.
		//Shader should implement it's own depth calculation.
		//Based on http://chaosinmotion.com/blog/?p=555

		y1 = 1 / tan(0.5f * fov);
		x0 = -1 * y1 / aspect;
		z2 = 0;
		z3 = near;
		w2 = 1;
	} else {
		//Projection perspective with -1 to 0 depth mapping:
		//Based on article from: http://outerra.blogspot.com.au/2012/11/maximizing-depth-buffer-range-and.html

		scalar frustumDepth = far-near;
		scalar oneOverDepth = 1 / frustumDepth;

		y1 = 1 / tan(0.5f * fov);
		x0 = -1 * y1 / aspect;
		z2 = near * oneOverDepth;
		z3 = -far * near * oneOverDepth;
		w2 = 1;


	}
	/* Normal projection
	 * scalar frustumDepth = far-near;
		scalar oneOverDepth = 1 / frustumDepth;

		y1 = 1 / tan(0.5f * fov);
		x0 = -1 * y1 / aspect;
		z2 = (near+far) * oneOverDepth;
		z3 = (-2*far * near) * oneOverDepth;
		w2 = 1;
	 */

	manMat4.create(mat,
				   x0,  0,  0,  0,
				    0, y1,  0,  0,
					0,  0, z2, z3,
					0,  0, w2, 0);


	Vec4 nv = manVec4.create(NULL, 0, 0, near, 1);
	Vec4 fv = manVec4.create(NULL, 0, 0, far, 1);

	Vec4 rn = manMat4.postMulVec4(mat, &nv);
	Vec4 rf = manMat4.postMulVec4(mat, &fv);
	printf("Near: %f\n", rn.z/rn.w);
	printf("Far: %f\n", rf.z/rf.w);

	manStack.push(stack, mat);
}

/**
 * Removes the next matrix in the current stack and returns it.
 * @return The old next matrix in the current stack.
 */
static Mat4* pop(MatrixManager* manager) {
	Stack* stack = manager->stacks[manager->matMode];
	if (stack->elementSize>0)
		return (Mat4*) manStack.pop(stack);
	else
		return NULL;
}

/**
 * Rotates the next element in current stack by the given angle around the given axis.
 * @param angle The angle to rotate by in radians.
 * @param axis The axis to rotate around (should be normalized)
 */
static void rotate(MatrixManager* manager, scalar angle, Vec3 axis) {
	*peek(manager) = manMat4.affRotate(peek(manager), angle, &axis);
}

/**
 * Translates the next element in the current stack by the given vector.
 * @param translation The vector containing how far to translate the matrix by along each axis.
 */
static void translate(MatrixManager* manager, Vec3 translation) {
	*peek(manager) = manMat4.affTranslate(peek(manager), &translation);
}

/**
 * Scales the next element in the current stack by the given vector.
 * @param scale The vector containing how much each axis should be scaled by.
 */
static void scale(MatrixManager* manager, Vec3 scale) {
	*peek(manager) = manMat4.affScale(peek(manager), &scale);
}

/**
 * Multiplies the next matrix in the current stack by the given matrix.
 * Useful for quarternion rotations.
 * @param matrix The matrix to multiply with.
 */
static void mult(MatrixManager* manager, Mat4* matrix) {
	if (matrix!=NULL) {
		//Apply the matrix transformation to the matrix
		(*peek(manager)) = manMat4.mul(peek(manager), matrix);
	}
}

static void clear(MatrixManager* manager) {
	for(int i = 0; i<MATRIX_MODE_COUNT; i++)
		manStack.delete(manager->stacks[i]);

	free(manager->stacks);
}

static void delete(MatrixManager* manager) {
	for(int i = 0; i < MATRIX_MODE_COUNT; i++) {
		manager->matMode = i;

		Mat4* ptr = pop(manager);
		while(ptr!=NULL) {
			free(ptr);
			ptr = pop(manager);
		}

		manStack.delete(manager->stacks[i]);
		free(manager->stacks[i]);
	}

}

const MatrixManagerManager manMatMan = {new, setMode, peek, peekStack, push, pushMat4, pushIdentity, pushPerspective, pop, rotate, translate, scale, mult, clear, delete};
