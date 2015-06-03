#ifndef COH_STACK_H
#define COH_STACK_H

#include <stdint.h>

/**
 *	Stack data structure.
 */
struct Stack_s {
	void** contents;
	uint64_t arraySize;
	uint64_t elementSize;
};

typedef struct Stack_s Stack;

/**
 *	Manager used to manage Stack data structures.
 */	
struct StackManager_s {
	Stack*(* new)();

	void (* push)(Stack*, void*);
	void*(* peek)(Stack*);
	void*(* pop)(Stack*);
	void (* trim)(Stack*);

	void (* delete)(Stack*);
};


typedef struct StackManager_s StackManager;
extern const StackManager manStack;

#endif /* UTIL_STACK_H_ */
