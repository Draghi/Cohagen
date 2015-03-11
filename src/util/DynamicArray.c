#include <stdlib.h>
#include <assert.h>

#include "DynamicArray.h"

static void *getDynamicArrayElement(const DynamicArray *const array, unsigned int index);
static void appendDynamicArrayElement(DynamicArray *array, void *const element);

/**
 *  Creates a new dynamic array with the DynamicArray provided and
 *  the given initial capacity.
 *
 *  @param  array               pointer to DynamicArray, address of new array.
 *  @param  initialCapacity     unsigned int, initial size of array in memory.
 *  @param  elementSize         unsigned int, size of each element in the array.
 */
void newDynamicArray(DynamicArray *const array, unsigned int initialCapacity, unsigned int elementSize) {
    array->size = 0;
    array->capacity = initialCapacity;
    array->capacityExpansionRate = initialCapacity;
    array->elementSize = elementSize;
    array->contents = calloc(initialCapacity, sizeof(void *));

    array->get = getDynamicArrayElement;
    array->append = appendDynamicArrayElement;
}

/**
 *  Reset the structure and free the memory associated with it.
 *  Note that this does not mean pointers passed to the array will be freed,
 *  they will not. Only the memory used by this struct will be freed.
 *
 *  @param  array   pointer to DynamicArray to clear.
 */
void deleteDynamicArray(DynamicArray *const array) {
    // Free array of elements
    free(array->contents);

    // Reset members
    array->size = 0;
    array->capacity = 0;
    array->capacityExpansionRate = 0;
    array->elementSize = 0;
}

/**
 *  Attempts to return element at given index in array.
 *  Method will fail if the index given exceeds the size of the array.
 *
 *  @param  array   const pointer to const DynamicArray to get element from.
 *  @param  index   unsigned int, index to retrieve.
 *  @return         pointer to void, pointer to element retrieved.
 */
static void *getDynamicArrayElement(const DynamicArray *const array, unsigned int index) {
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
static void appendDynamicArrayElement(DynamicArray *array, void *const element) {
    if (array->size + 1 > array->capacity) {
        array->capacity += array->capacityExpansionRate;

        array->contents = realloc(array->contents, array->capacity*sizeof(void *));
    }

    array->contents[array->size] = element;
    ++(array->size);
}
