#ifndef UTIL_DYNAMIC_ARRAY_H
#define UTIL_DYNAMIC_ARRAY_H

/**
 *  A DynamicArray (ie. vector) pseudo-class.
 *
 *  
 *  All interaction with this class should happen via the supplied functions.
 *  Do not attempt to modify member variables directly, although they can be assumed
 *  to be correct (for the purposes of retrieving information, eg. size of array).
 */
typedef struct DynamicArray_s {
    /**
     *  Attempts to return element at given index in array.
     *  Method will fail if the index given exceeds the size of the array.
     *
     *  @param  array   const pointer to const DynamicArray to get element from.
     *  @param  index   unsigned int, index to retrieve.
     *  @return         pointer to void, pointer to element retrieved.
     */
    void *(*get)(const struct DynamicArray_s *const, unsigned int);

    /**
     *  Append an element to the end of the DynamicArray. The size and capacity of the
     *  array will automatically be adjusted.
     *
     *  @param  array       pointer to DynamicArray to append element to.
     *  @param  element     const pointer to const void, element to append.
     */
    void (*append)(struct DynamicArray_s *, void *const);

    /**
     *  Size of array (number of elements in array.)
     */
    unsigned int size;

    /**
     *  Size of each element in array. Eg. DynamicArray of ints
     *  has the element size: sizeof(int).
     */
    unsigned int elementSize;

    /**
     *  Capacity of array, how many elements we can currently
     *  fit in the array.
     */
    unsigned int capacity;

    /**
     *  Rate at which to expand the array when needed (num of elements).
     */
    unsigned int capacityExpansionRate;

    /**
     *  Contents of array.
     */
    void **contents;
} DynamicArray;

void newDynamicArray(DynamicArray *array, unsigned int initialCapacity, unsigned int elementSize);
void clearDynamicArray(DynamicArray *array);

#endif