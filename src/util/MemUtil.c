#include<stdlib.h>
#include<stdint.h>

#include "MemUtil.h"

void** malloc2D(uint64_t elementSize, uint64_t xCount, uint64_t yCount) {
	void** array;
	array = malloc(xCount*sizeof(void*));

	for(uint64_t i = 0; i<xCount; i++)
		array[i] = malloc(yCount*elementSize);

	return array;
}

void free2D(void** array, uint64_t xCount) {
	for(uint64_t i = 0; i<xCount; i++)
		free(array[i]);

	free(array);
}

const MemUtil memUtil = {malloc2D, free2D};
