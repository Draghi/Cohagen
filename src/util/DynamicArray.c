#include <stdlib.h>
#include <assert.h>

#include "DynamicArray.h"

static void *getDynamicArrayElement(const DynamicArray *const array, unsigned int index);
static void appendDynamicArrayElement(DynamicArray *array, void *const element);

void newDynamicArray(DynamicArray *const array, unsigned int initialCapacity, unsigned int elementSize) {
    array->size = 0;
    array->capacity = initialCapacity;
    array->capacityExpansionRate = initialCapacity;
    array->elementSize = elementSize;
    array->contents = calloc(initialCapacity, sizeof(void *));

    array->get = getDynamicArrayElement;
    array->append = appendDynamicArrayElement;
}

void deleteDynamicArray(DynamicArray *const array) {
    // Free array of elements
    free(array->contents);

    // Reset members
    array->size = 0;
    array->capacity = 0;
    array->capacityExpansionRate = 0;
    array->elementSize = 0;
}

void freeDynamicArrayContents(DynamicArray *const array) {
    for (int i = 0; i < array->size; ++i) {
        free(array->get(array, i));
    }

    array->size = 0;
}

static void *getDynamicArrayElement(const DynamicArray *const array, unsigned int index) {
    assert(index < array->size && "Attempt to index out-of-bounds on DynamicArray.");

    return (array->contents[index]);
}

static void appendDynamicArrayElement(DynamicArray *array, void *const element) {
    if (array->size + 1 > array->capacity) {
        array->capacity += array->capacityExpansionRate;

        array->contents = realloc(array->contents, array->capacity*sizeof(void *));
    }

    array->contents[array->size] = element;
    ++(array->size);
}
