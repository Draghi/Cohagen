#include "MatrixManager.h"

#include "../util/Stack.h"
#include "../math/Mat4.h"
#include "../math/Precision.h"

#include <stdlib.h>
#include <stdbool.h>

#define MATRIX_MODE_COUNT 3

Stack** stacks;
uint32_t matMode;

static init() {
	stacks = malloc(sizeof(void*)*MATRIX_MODE_COUNT);
	for(int i = 0; i < MATRIX_MODE_COUNT; i++) {
		stacks[i] = manStack.new();
	}

	matMode = 0;
}

static bool setMode(uint32_t mode) {
	if (mode<MATRIX_MODE_COUNT) {
		matMode = mode;
		return true;
	} else {
		return false;
	}
}

static void pushMatrix(Mat4* matrix) {
	Stack* stack = stacks[matMode];
	manStack.push(stack, matrix);
}

static void push() {
	Stack* stack = stacks[matMode];
	manStack.push(stack, manStack.peek(stack));
}

static Mat4* pop() {
	Stack* stack = stacks[matMode];
	return (Mat4*) manStack.pop(stack);
}

static Mat4* peek() {
	Stack* stack = stacks[matMode];
	return (Mat4*) manStack.peek(stack);
}

static void rotate(scalar angle, scalar x, scalar y, scalar z) {
	Mat4* matrix = peek();

	/*matrix->data[0].x =
	matrix->data[0].y =
	matrix->data[0].z =
	matrix->data[0].w =*/
}
