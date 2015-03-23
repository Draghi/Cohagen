#include "MatrixManager.h"

#include "../math/Precision.h"
#include "../math/Vec3.h"
#include "../math/Mat4.h"
#include "../util/Stack.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/** The Projection Matrix Stack **/
const int MATRIX_MODE_PROJECTION = 0x0000;

/** The View Matrix Stack **/
const int MATRIX_MODE_VIEW  = 0x0001;

/** The Model Matrix Stack **/
const int MATRIX_MODE_MODEL = 0x0002;

/** The Number of Matrix Stacks **/
const int MATRIX_MODE_COUNT = 3;

/** The stacks where matricies are stored */
Stack** stacks;

/** The current stack mode the manager is in */
uint32_t matMode;

/** Used so we don't initialize twice. */
bool initialized = false;

/**
 * Should be called at the beginning to prepare the matrix manager.
 */
static void init() {
	if (!initialized) {
		stacks = malloc(sizeof(void*)*MATRIX_MODE_COUNT);
		for(int i = 0; i < MATRIX_MODE_COUNT; i++) {
			stacks[i] = manStack.new();
		}

		matMode = 0;
		initialized = true;
	}
}

/**
 * Changes which matrix stack to perform operations on.
 * @param mode - The mode to change to (MATRIX_MODE_PROJECTION eg.)
 * @return Whether the mode was changed or not.
 */
static bool setMode(uint32_t mode) {
	if (mode<MATRIX_MODE_COUNT) {
		matMode = mode;
		return true;
	} else {
		return false;
	}
}

/**
 * Returns the next element in the current stack, without removing it.
 * @return The next element in the current stack, without removing it. NULL if the stack is empty.
 */
static Mat4* peek() {
	Stack* stack = stacks[matMode];
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
static Mat4* peekStack(uint32_t mode) {
	if (mode<MATRIX_MODE_COUNT) {
		Stack* stack = stacks[mode];
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
static void pushMat4(Mat4* mat) {
	if (mat!=NULL) {
		Stack* stack = stacks[matMode];
		Mat4* matClone = malloc(sizeof(Mat4));
		*matClone = createMat4Mat4(peek());
		manStack.push(stack, matClone);
	}
}

/**
 * Copies the next matrix in the stack and pushes it onto the stack.
 */
static void push() {
	pushMat4(peek());
}

/**
 * Pushes a new identity matrix onto the stack.
 */
static void pushIdentity() {
	Stack* stack = stacks[matMode];
	Mat4* mat = malloc(sizeof(Mat4));
	*mat = createMat4Leading(1);
	manStack.push(stack, mat);
}

/**
 * Pushes a new perspective matrix onto the current stack.
 * @param fov The horizontal fov in radians.
 * @param aspect The aspect ratio of the screen.
 * @param near The closest clipping plane's distance (Shouldn't be 0)
 * @param far The farthest clipping plane's distance.
 */
static void pushPerspective(scalar fov, scalar aspect, scalar near, scalar far) {
	Mat4 mat = createMat4Leading(1);
	scalar x_scale = (scalar) (1/tan(0.5f * fov));

	mat.data[0].x = x_scale;
	mat.data[1].y = x_scale/aspect;
	mat.data[2].z = -((far+near)/(far-near));
	mat.data[3].w = 0;

	mat.data[2].w = -1;
	mat.data[3].z = -((2*near*far)/(far-near));

	pushMat4(&mat);
}

/**
 * Removes the next matrix in the current stack and returns it.
 * @return The old next matrix in the current stack.
 */
static Mat4* pop() {
	Stack* stack = stacks[matMode];
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
static void rotate(scalar angle, Vec3 axis) {
	Mat4 src = *peek(); //Copy the data onto the stack for faster read access (no dereferencing)
	Mat4 dest;

	//Pre-calculations to streamline matrix transformation.
	scalar aCos = (scalar) cos(angle);
	scalar aSin = (scalar) sin(angle);
	scalar invC = 1.0f - aCos;

	scalar xx = axis.x*axis.x*invC;
	scalar yy = axis.y*axis.y*invC;
	scalar zz = axis.z*axis.z*invC;

	scalar xy = axis.x*axis.y*invC;
	scalar yz = axis.y*axis.z*invC;
	scalar xz = axis.x*axis.z*invC;

	scalar xsin = axis.x*aSin;
	scalar ysin = axis.y*aSin;
	scalar zsin = axis.z*aSin;


	//Transform matrix by doing an explicit Vec3 multiply to each component.
	//Explicit so we avoid the overhead of function calls. We want this to be very very fast.
	scalar m00 = xx + aCos;
	scalar m01 = xy + zsin;
	scalar m02 = xz - ysin;
	dest.data[0].x = src.data[0].x * m00 + src.data[1].x * m01 + src.data[2].x * m02;
	dest.data[0].y = src.data[0].y * m00 + src.data[1].y * m01 + src.data[2].y * m02;
	dest.data[0].z = src.data[0].z * m00 + src.data[1].z * m01 + src.data[2].z * m02;
	dest.data[0].w = src.data[0].w * m00 + src.data[1].w * m01 + src.data[2].w * m02;

	scalar m10 = xy - zsin;
	scalar m11 = yy + aCos;
	scalar m12 = yz + xsin;
	dest.data[1].x = src.data[0].x * m10 + src.data[1].x * m11 + src.data[2].x * m12;
	dest.data[1].y = src.data[0].y * m10 + src.data[1].y * m11 + src.data[2].y * m12;
	dest.data[1].z = src.data[0].z * m10 + src.data[1].z * m11 + src.data[2].z * m12;
	dest.data[1].w = src.data[0].w * m10 + src.data[1].w * m11 + src.data[2].w * m12;

	scalar m20 = xz + ysin;
	scalar m21 = yz - xsin;
	scalar m22 = zz + aCos;
	dest.data[2].x = src.data[0].x * m20 + src.data[1].x * m21 + src.data[2].x * m22;
	dest.data[2].y = src.data[0].y * m20 + src.data[1].y * m21 + src.data[2].y * m22;
	dest.data[2].z = src.data[0].z * m20 + src.data[1].z * m21 + src.data[2].z * m22;
	dest.data[2].w = src.data[0].w * m20 + src.data[1].w * m21 + src.data[2].w * m22;

	//Apply the matrix transformation to the matrix
	(*peek()) = dest;
}

/**
 * Translates the next element in the current stack by the given vector.
 * @param translation The vector containing how far to translate the matrix by along each axis.
 */
static void translate(Vec3 translation) {
	Mat4 src = *peek(); //Copy the data onto the stack for faster read access (no dereferencing)
	Mat4 dest;

	dest.data[3].x += src.data[0].x * translation.x + src.data[1].x * translation.y + src.data[2].x * translation.z;
	dest.data[3].y += src.data[0].y * translation.x + src.data[1].y * translation.y + src.data[2].y * translation.z;
	dest.data[3].z += src.data[0].z * translation.x + src.data[1].z * translation.y + src.data[2].z * translation.z;
	dest.data[3].w += src.data[0].w * translation.x + src.data[1].w * translation.y + src.data[2].w * translation.z;

	//Apply the matrix transformation to the matrix
	(*peek()) = dest;
}

/**
 * Scales the next element in the current stack by the given vector.
 * @param scale The vector containing how much each axis should be scaled by.
 */
static void scale(Vec3 scale) {
	Mat4 src = *peek(); //Copy the data onto the stack for faster read access (no dereferencing)
	Mat4 dest;

	dest.data[0].x *= src.data[0].x * scale.x;
	dest.data[0].y *= src.data[0].y * scale.x;
	dest.data[0].z *= src.data[0].z * scale.x;
	dest.data[0].w *= src.data[0].w * scale.x;

	dest.data[1].x *= src.data[1].x * scale.y;
	dest.data[1].y *= src.data[1].y * scale.y;
	dest.data[1].z *= src.data[1].z * scale.y;
	dest.data[1].w *= src.data[1].w * scale.y;

	dest.data[2].x *= src.data[2].x * scale.z;
	dest.data[2].y *= src.data[2].y * scale.z;
	dest.data[2].z *= src.data[2].z * scale.z;
	dest.data[2].w *= src.data[2].w * scale.z;

	//Apply the matrix transformation to the matrix
	(*peek()) = dest;
}

/**
 * Multiplies the next matrix in the current stack by the given matrix.
 * Useful for quarternion rotations.
 * @param matrix The matrix to multiply with.
 */
static void mult(Mat4* matrix) {
	if (matrix!=NULL) {
		//Apply the matrix transformation to the matrix
		(*peek()) = mulMat4Mat4(peek(), matrix);
	}
}

const MatrixManager manMat = {init, setMode, peek, peekStack, push, pushMat4, pushIdentity, pushPerspective, pop, rotate, translate, scale, mult};
