#include "BST.h"

#include <stdlib.h>
#include <math.h>

#define BST_ORDER_PRE 0x0000
#define BST_ORDER_IN 0x0001
#define BST_ORDER_POST 0x0002

const int BST_LESS = 0x0000;
const int BST_EQUAL = 0x0001;
const int BST_GREATER = 0x0002;

static void traversePre(BST* bst, BSTDataArray* dest){
	if (bst!=NULL) {
		(*dest->data) = bst->data;
		dest->data++;
		traversePre(bst->left, dest);
		traversePre(bst->right, dest);
	}
}

static void traverseIn(BST* bst, BSTDataArray* dest){
	if (bst!=NULL) {
		traverseIn(bst->left, dest);
		*dest->data = bst->data;
		dest->data++;
		traverseIn(bst->right, dest);
	}
}

static void traversePost(BST* bst, BSTDataArray* dest){
	if (bst!=NULL) {
		traversePost(bst->left, dest);
		traversePost(bst->right, dest);
		*dest->data = bst->data;
		dest->data++;
	}
}

static BST* getPre(BST* bst, int* id){
	if (bst!=NULL) {
		if (id == 0)
			return bst;
		id--;

		BST* res = getPre(bst->left, id);

		if (res!=NULL)
			return res;

		res = getPre(bst->right, id);
		if (res!=NULL)
			return res;
	}

	return NULL;
}

static BST* getIn(BST* bst, int* id){
	if (bst!=NULL) {
		BST* res = getIn(bst->left, id);

		if (res!=NULL)
			return res;

		if (id == 0)
			return bst;
		id--;

		res = getIn(bst->right, id);
		if (res!=NULL)
			return res;
		}

	return NULL;
}

static BST* getPost(BST* bst, int* id){
	if (bst!=NULL) {
		BST* res = getPre(bst->left, id);

		if (res!=NULL)
			return res;

		res = getPre(bst->right, id);
		if (res!=NULL)
			return res;

		if (id == 0)
			return bst;
		id--;
	}

	return NULL;
}

static BST* searchPre(BST* bst, void* search, CompareCallback* compareCallback){
	if (bst!=NULL) {
		if (compareCallback(bst->data, search) == BST_EQUAL)
			return bst;

		BST* res = searchPre(bst->left, search, compareCallback);

		if (res!=NULL)
			return res;

		res = searchPre(bst->right, search, compareCallback);
		if (res!=NULL)
			return res;
	}

	return NULL;
}

static BST* searchIn(BST* bst, void* search, CompareCallback* compareCallback){
	if (bst!=NULL) {
		BST* res = searchIn(bst->left, search, compareCallback);

		if (res!=NULL)
			return res;

		if (compareCallback(bst->data, search) == BST_EQUAL)
			return bst;

		res = searchIn(bst->right, search, compareCallback);
		if (res!=NULL)
			return res;
		}

	return NULL;
}

static BST* searchPost(BST* bst, void* search, CompareCallback* compareCallback){
	if (bst!=NULL) {
		BST* res = searchPre(bst->left, search, compareCallback);

		if (res!=NULL)
			return res;

		res = searchPre(bst->right, search, compareCallback);
		if (res!=NULL)
			return res;

		if (compareCallback(bst->data, search) == BST_EQUAL)
			return bst;
	}

	return NULL;
}

static void countDepth(BST* bst, int* counter){
	if (bst!=NULL) {
		countDepth(bst->left, counter);
		countDepth(bst->right, counter);
		(*counter)++;
	}
}

static BST* new(void* data) {
	BST* bst = malloc(sizeof(BST));

	bst->data = data;
	bst->left = NULL;
	bst->right = NULL;
	bst->parent = NULL;

	return bst;
}

static BSTDataArray* traverse(BST* bst, int order) {
	BSTDataArray* dest = malloc(sizeof(dest));
	countDepth(bst, &dest->size);
	dest->data = malloc(sizeof(void*)*dest->size);
	void** data = dest->data;

	switch(order) {
		case(BST_ORDER_PRE):
			traversePre(bst, dest);
			break;
		case(BST_ORDER_IN):
			traverseIn(bst, dest);
			break;
		case(BST_ORDER_POST):
			traversePost(bst, dest);
			break;
	}
	dest->data = data;

	return dest;
}

static BST* get(BST* bst, int id, int order) {
	BST* res = NULL;

	switch(order) {
		case(BST_ORDER_PRE):
			res = getPre(bst, &id);
			break;
		case(BST_ORDER_IN):
			res = getIn(bst, &id);
			break;
		case(BST_ORDER_POST):
			res = getPost(bst, &id);
			break;
	}

	return res;
}

static BST* search(BST* bst, int order, void* search, CompareCallback* compareCallback) {
	BST* res = NULL;

	switch(order) {
		case(BST_ORDER_PRE):
			res = searchPre(bst, search, compareCallback);
			break;
		case(BST_ORDER_IN):
			res = searchIn(bst, search, compareCallback);
			break;
		case(BST_ORDER_POST):
			res = searchPost(bst, search, compareCallback);
			break;
	}

	return res;
}

static void remove(BST* bst) {
	BST* nextHighest = NULL;

	if (bst->right!=NULL)
		nextHighest = get(bst->right, 0, BST_ORDER_IN);
	else if (bst->left!=NULL) {
		int depth = 0;
		countDepth(bst, &depth);
		nextHighest = get(bst->right, depth-1, BST_ORDER_IN);
	} else
		nextHighest = bst;


	if (nextHighest->parent!=NULL) {
		if (nextHighest->parent->left == nextHighest) {
			nextHighest->parent->left = NULL;
		} else if (nextHighest->parent->right == nextHighest) {
			nextHighest->parent->right = NULL;
		}
	}

	bst->data = nextHighest->data;
}

static bool add(BST* bst, void* data, CompareCallback* compareCallback) {
	if (bst->data == NULL) {
		bst->data = data;
	} else {
		int compare = compareCallback(bst->data, data);

		if (compare == BST_LESS) {
			if (bst->left == NULL) {
				bst->left = new(data);
				bst->left->parent = bst;
				return true;
			} else return add(bst->left, data, compareCallback);
		} else if (compare == BST_GREATER) {
			if (bst->right == NULL) {
				bst->right = new(data);
				bst->right->parent = bst;
				return true;
			} else return add(bst->right, data, compareCallback);
		}
	}
	return false;
}

static void deleteInternal(BST* bst) {
	if (bst!=NULL) {
		deleteInternal(bst->left);
		deleteInternal(bst->right);
		free(bst);
	}
}

static void delete(BST* bst) {
	deleteInternal(bst->left);
	deleteInternal(bst->right);
}

BSTManager manBST = {new, traverse, add, remove, get, search, delete};
