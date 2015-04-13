#ifndef UTIL_DYNAMIC_ARRAY_H
#define UTIL_DYNAMIC_ARRAY_H

/**
 *  A DynamicArray (ie. vector) pseudo-class.
 *
 *  All interaction with this class should happen via the supplied functions.
 *  Do not attempt to modify member variables directly, although they can be assumed
 *  to be correct (for the purposes of retrieving information, eg. size of array).
 */
typedef struct DynamicArray_s {
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

typedef struct DynamicArrayManager_s {
    /**
     *  Creates a new dynamic array with the DynamicArray provided and
     *  the given initial capacity.
     *
     *  @param  initialCapacity     unsigned int, initial size of array in memory.
     *  @param  elementSize         unsigned int, size of each element in the array.
     */
    DynamicArray *(*new)(unsigned int initialCapacity, unsigned int elementSize);

    /**
     *  Reset the structure and free the memory associated with it.
     *  Note that this does not mean pointers passed to the array will be freed,
     *  they will not. Only the memory used by this struct will be freed.
     *
     *  @param  array   pointer to DynamicArray to clear.
     */
    void (*delete)(DynamicArray *array);

    /**
     *  Cycle through the contents of the array and frees each element, also sets size of array to 0.
     *  This function will crash the program if two elements of the array point to the
     *  same location in memory or any pointer points to stack memory.
     *
     *  @params     array   const pointer to Dynamic Array, array to free contents of.
     */
    void (*freeContents)(DynamicArray *const array);

    /**
     *  Attempts to return element at given index in array.
     *  Method will fail if the index given exceeds the size of the array.
     *
     *  @param  array   const pointer to const DynamicArray to get element from.
     *  @param  index   unsigned int, index to retrieve.
     *  @return         pointer to void, pointer to element retrieved.
     */
    void *(*get)(const struct DynamicArray_s *const array, unsigned int index);

    /**
     *  Append an element to the end of the DynamicArray. The size and capacity of the
     *  array will automatically be adjusted.
     *
     *  @param  array       pointer to DynamicArray to append element to.
     *  @param  element     const pointer to const void, element to append.
     */
    void (*append)(struct DynamicArray_s *array, void *const element);

} DynamicArrayManager;

// typedef struct DynamicArray_s {
//     /**
//      *  Attempts to return element at given index in array.
//      *  Method will fail if the index given exceeds the size of the array.
//      *
//      *  @param  array   const pointer to const DynamicArray to get element from.
//      *  @param  index   unsigned int, index to retrieve.
//      *  @return         pointer to void, pointer to element retrieved.
//      */
//     void *(*get)(const struct DynamicArray_s *const, unsigned int);

//     /**
//      *  Append an element to the end of the DynamicArray. The size and capacity of the
//      *  array will automatically be adjusted.
//      *
//      *  @param  array       pointer to DynamicArray to append element to.
//      *  @param  element     const pointer to const void, element to append.
//      */
//     void (*append)(struct DynamicArray_s *, void *const);

//     /**
//      *  Size of array (number of elements in array.)
//      */
//     unsigned int size;

//     /**
//      *  Size of each element in array. Eg. DynamicArray of ints
//      *  has the element size: sizeof(int).
//      */
//     unsigned int elementSize;

//     /**
//      *  Capacity of array, how many elements we can currently
//      *  fit in the array.
//      */
//     unsigned int capacity;

//     *
//      *  Rate at which to expand the array when needed (num of elements).
     
//     unsigned int capacityExpansionRate;

//     /**
//      *  Contents of array.
//      */
//     void **contents;
// } DynamicArray;

// /**
//  *  Creates a new dynamic array with the DynamicArray provided and
//  *  the given initial capacity.
//  *
//  *  @param  array               pointer to DynamicArray, address of new array.
//  *  @param  initialCapacity     unsigned int, initial size of array in memory.
//  *  @param  elementSize         unsigned int, size of each element in the array.
//  */
//  void newDynamicArray(DynamicArray *array, unsigned int initialCapacity, unsigned int elementSize);

// /**
//  *  Reset the structure and free the memory associated with it.
//  *  Note that this does not mean pointers passed to the array will be freed,
//  *  they will not. Only the memory used by this struct will be freed.
//  *
//  *  @param  array   pointer to DynamicArray to clear.
//  */
//  void deleteDynamicArray(DynamicArray *array);

// /**
//  *  Cycle through the contents of the array and frees each element, also sets size of array to 0.
//  *  This function will crash the program if two elements of the array point to the
//  *  same location in memory.
//  *
//  *  @params     array   const pointer to Dynamic Array, array to free contents of.
//  */
// void freeDynamicArrayContents(DynamicArray *const array);

extern const DynamicArrayManager manDynamicArray;

#endif