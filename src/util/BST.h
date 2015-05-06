#ifndef COH_BST_H
#define COH_BST_H

#include <stdbool.h>

typedef bool CompareCallback(void* oldData, void* newData);

typedef struct BST_s BST;

typedef struct BST_s {
	void* data;
	BST* left;
	BST* right;
	BST* parent;
} BST;

typedef struct BSTDataArray_s {
	void** data;
	int size;
} BSTDataArray;

typedef struct BSTManager_s {
	BST*(* new)(void* data);
	BSTDataArray*(* traverse)(BST* bst, int order);
	bool(* add)(BST* bst, void* data, CompareCallback* compareCallback);
	void(* remove)(BST* bst);
	BST*(* get)(BST* bst, int id, int order);
	BST*(* search)(BST* bst, int order, void* find, CompareCallback* compareCallback);
	void(* delete)(BST* bst);
} BSTManager;

#endif
