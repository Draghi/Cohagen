#ifndef UTIL_DYNAMIC_FLOAT_ARRAY_H
#define UTIL_DYNAMIC_FLOAT_ARRAY_H

/**
 *  Dynamically-sized array class capable of holding float.
 */
typedef struct DynamicFloatArray_s {
    /**
     *  Attempts to return element at given index in array.
     *  Method will fail if the index given exceeds the size of the array.
     *
     *  @param  array   const pointer to const DynamicArray to get element from.
     *  @param  index   unsigned int, index to retrieve.
     *  @return         float, element retrieved.
     */
    float (*get)(const struct DynamicFloatArray_s *const, unsigned int);

    /**
     *  Append an element to the end of the DynamicArray. The size and capacity of the
     *  array will automatically be adjusted.
     *
     *  @param  array       pointer to DynamicArray to append element to.
     *  @param  element     float, element to append.
     */
    void (*append)(struct DynamicFloatArray_s *, float);

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

    float *contents;
} DynamicFloatArray;

void newDynamicFloatArray(DynamicFloatArray *array, unsigned int initialCapacity);
void deleteDynamicFloatArray(DynamicFloatArray *array);

#endif