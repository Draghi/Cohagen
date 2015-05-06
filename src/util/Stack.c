#include "Stack.h"

#include <stdlib.h>

////////////////////////
// Internal Functions //
////////////////////////

static void changeSize(Stack* stack, uint64_t size) {
	stack->contents = realloc(stack->contents, size);
	stack->arraySize = size;
}

static void set(Stack* stack, void* data, uint64_t pos) {
	if (pos>=(stack->arraySize)) {
		changeSize(stack, pos+1);
	} else {
		if(stack->contents[pos]!=NULL)
			free(stack->contents[pos]);
	}

	stack->contents[pos] = data;

	if (stack->elementSize < pos+1)
		stack->elementSize = pos+1;
}

static void remove(Stack* stack, uint64_t pos) {
	if (pos<stack->elementSize) {
		stack->elementSize--;

		if (pos!=(stack->elementSize)) {
			for(int i = pos; i<(stack->elementSize); i++) {
				stack->contents[i] = stack->contents[i+1];
			}
		}

		//manStack.trim(stack);
	}
}

/////////////////////
// Stack Functions //
/////////////////////
static Stack* new() {
	Stack* stack = malloc(sizeof(Stack));
	stack->contents = malloc(sizeof(void**));
	stack->contents[0] = NULL;
	stack->arraySize = 1;
	stack->elementSize = 0;

	return stack;
}

static void push(Stack* stack, void* data) {
	set(stack, data, stack->elementSize);
}

static void* peek(Stack* stack) {
	if (stack->elementSize>0)
		return stack->contents[stack->elementSize-1];

	return NULL;
}

static void* pop(Stack* stack) {
	void* element = peek(stack);

	remove(stack, stack->elementSize-1);

	return element;
}

static void trim(Stack* stack) {
	if (stack->elementSize<stack->arraySize)
		changeSize(stack, stack->elementSize);
}

static void delete(Stack* stack) {
	free(stack->contents);
}

const StackManager manStack = {new, push, peek, pop, trim, delete};
