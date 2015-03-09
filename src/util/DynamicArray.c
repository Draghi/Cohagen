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
 *  Loops through each element in the DynamicArray, freeing the memory associated with it.
 *  The given dynamic array is ready to be used again with "newDynamicArray" after this function is called.
 *
 *  Note that this function only works with heap memory. Do not attempt to append a pointer to stack memory
 *  to this structure.
 *
 *  @param  array   pointer to DynamicArray to clear.
 */
void clearDynamicArray(DynamicArray *const array) {
    // Free each element
    for (unsigned int i = 0; i < array->size; ++i) {
        free(array->contents[i]);
    }

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

        array->contents = realloc(array->contents, array->capacity);
    }

    array->contents[array->size] = element;
    ++(array->size);
}

/**
 *  Exception to the rule. Use this function to append
 *  stack memory to the Dynamic Array.
 * 
 *  Memory is created on the heap with the same value as the
 *  given stack memory and a pointer to that heap memory is appended
 *  to the class.
 *
 *  @param  array           pointer to DynamicArray to append element to.
 *  @param  elementSize     unsigned int, size of element to append.
 *  @param  element         pointer to void, pointer to element on the stack.
 */
// static void appendStackMemory(struct DynamicArray_s *array, unsigned int elementSize, void *const element) {
//     // Copy element onto heap.
//     void *cloneElement = malloc(elementSize);
//     *cloneElement = *element;

//     // Append cloned element to array.
//     array->append(array, cloneElement);
// }