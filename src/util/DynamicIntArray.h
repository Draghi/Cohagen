#ifndef UTIL_DYNAMIC_INT_ARRAY_H
#define UTIL_DYNAMIC_INT_ARRAY_H

/**
 *  Dynamically-sized array class capable of holding int.
 */
typedef struct DynamicIntArray_s {
    /**
     *  Attempts to return element at given index in array.
     *  Method will fail if the index given exceeds the size of the array.
     *
     *  @param  array   const pointer to const DynamicArray to get element from.
     *  @param  index   unsigned int, index to retrieve.
     *  @return         int, element retrieved.
     */
    int (*get)(const struct DynamicIntArray_s *const, unsigned int);

    /**
     *  Append an element to the end of the DynamicArray. The size and capacity of the
     *  array will automatically be adjusted.
     *
     *  @param  array       pointer to DynamicArray to append element to.
     *  @param  element     int, element to append.
     */
    void (*append)(struct DynamicIntArray_s *, int);

    /**
     *  Size of array (number of elements in array.)
     */
    unsigned int size;

    /**
     *  Capacity of array, how many elements we can currently
     *  fit in the array.
     */
    unsigned int capacity;

    /**
     *  Rate at which to expand the array when needed (num of elements).
     */
    unsigned int capacityExpansionRate;

    int *contents;
} DynamicIntArray;

void newDynamicIntArray(DynamicIntArray *array, unsigned int initialCapacity);
void deleteDynamicIntArray(DynamicIntArray *array);

#endif
