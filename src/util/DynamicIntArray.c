#include <stdlib.h>
#include <assert.h>

#include "DynamicIntArray.h"

static int    getDynamicIntArrayElement(const DynamicIntArray *const array, unsigned int index);
static void     appendDynamicIntArrayElement(DynamicIntArray *array, int element);

/**
 *  Creates a new dynamic array with the DynamicArray provided and
 *  the given initial capacity.
 *
 *  @param  array               pointer to DynamicArray, address of new array.
 *  @param  initialCapacity     unsigned int, initial size of array in memory.
 *  @param  elementSize         unsigned int, size of each element in the array.
 */
void newDynamicIntArray(DynamicIntArray *const array, unsigned int initialCapacity) {
    array->size = 0;
    array->capacity = initialCapacity;
    array->capacityExpansionRate = initialCapacity;
    array->contents = calloc(initialCapacity, sizeof(int));

    array->get = getDynamicIntArrayElement;
    array->append = appendDynamicIntArrayElement;
}

/**
 *  Reset the structure and free the memory associated with it.
 *
 *  @param  array   pointer to DynamicArray to clear.
 */
void deleteDynamicIntArray(DynamicIntArray *const array) {
    // Free array of elements
    free(array->contents);

    // Reset members
    array->size = 0;
    array->capacity = 0;
    array->capacityExpansionRate = 0;
}

/**
 *  Attempts to return element at given index in array.
 *  Method will fail if the index given exceeds the size of the array.
 *
 *  @param  array   const pointer to const DynamicArray to get element from.
 *  @param  index   unsigned int, index to retrieve.
 *  @return         pointer to void, pointer to element retrieved.
 */
static int getDynamicIntArrayElement(const DynamicIntArray *const array, unsigned int index) {
    assert(index < array->size && "Attempt to index out-of-bounds on DynamicArray.");

    return (array->contents[index]);
}

/**
 *  Append an element to the end of the DynamicArray. The size and capacity of the
 *  array will automatically be adjusted.
 *
 *  @param  array       pointer to DynamicArray to append element to.
 *  @param  element     pointer to void, element to append.
 */
static void appendDynamicIntArrayElement(DynamicIntArray *array, int element) {
    if (array->size + 1 > array->capacity) {
        array->capacity += array->capacityExpansionRate;

        array->contents = realloc(array->contents, array->capacity*sizeof(int));
    }

    array->contents[array->size] = element;
    ++(array->size);
}
