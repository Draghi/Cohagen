#include "DynamicArray.h"

#include <stdlib.h>
#include <assert.h>

static void *get(const DynamicArray *const array, unsigned int index);
static void append(DynamicArray *array, void *const element);

static DynamicArray *new(unsigned int initialCapacity, unsigned int elementSize) {
    DynamicArray *array = malloc(sizeof(DynamicArray));

    array->size = 0;
    array->capacity = initialCapacity;
    array->capacityExpansionRate = initialCapacity;
    array->elementSize = elementSize;
    array->contents = calloc(initialCapacity, sizeof(void *));

    return array;
}

static void delete(DynamicArray *const array) {
    // Free array of elements
    free(array->contents);

    // Reset members
    array->size = 0;
    array->capacity = 0;
    array->capacityExpansionRate = 0;
    array->elementSize = 0;
}

static void freeDynamicArray(DynamicArray *const array) {
    for (int i = 0; i < array->size; ++i) {
        free(manDynamicArray.get(array, i));
    }

    array->size = 0;
}

static void *get(const DynamicArray *const array, unsigned int index) {
    assert(index < array->size && "Attempt to index out-of-bounds on DynamicArray.");

    return (array->contents[index]);
}

static void append(DynamicArray *array, void *const element) {
    if (array->size + 1 > array->capacity) {
        array->capacity += array->capacityExpansionRate;

        array->contents = realloc(array->contents, array->capacity*sizeof(void *));
    }

    array->contents[array->size] = element;
    ++(array->size);
}

const DynamicArrayManager manDynamicArray = {new, delete, free, get, append, freeDynamicArray};