#include "DynamicArray.h"

#include <stdlib.h>
#include <assert.h>

static char *get(const DynamicArray *const array, unsigned int index);
static void append(DynamicArray *array, void *const element);

static DynamicArray *new(unsigned int initialCapacity, unsigned int elementSize) {
    DynamicArray *array = malloc(sizeof(DynamicArray));

    array->size = 0;
    array->capacity = initialCapacity;
    array->capacityExpansionRate = initialCapacity;
    array->elementSize = elementSize;
    array->contents = (char *)calloc(initialCapacity*elementSize, sizeof(char));

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

    // free(array);
}

static char *get(const DynamicArray *const array, unsigned int index) {
    assert(index < array->size && "Attempt to index out-of-bounds on DynamicArray.");

    return (&array->contents[index * array->elementSize]);
}

static void append(DynamicArray *array, void *const element) {
    if (array->size + 1 > array->capacity) {
        array->capacity += array->capacityExpansionRate;

        array->contents = realloc(array->contents, array->capacity*array->elementSize);
    }

    // Convert void pointer to char pointer
    char *cp = (char *) element;

    // Move to the end of the array, put first byte there,
    // move along a byte and put next byte there etc...
    for (int i = array->elementSize; i > 0; --i) {
        array->contents[((array->size + 1) * array->elementSize) - i] = *cp;
        ++cp;
    }

    ++(array->size);
}

const DynamicArrayManager manDynamicArray = {new, delete, get, append};
