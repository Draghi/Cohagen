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

uint64_t fillBytes(void* dest, uint8_t* bytes, uint64_t offset, uint64_t size) {
	int8_t dir = 1;
	uint64_t writeOff = 0;

	uint64_t readOff = offset;
	uint64_t maxReadOff = size + offset;
	uint8_t *destBytes = (uint8_t *) dest;

	while(readOff<maxReadOff){
		destBytes[writeOff] = bytes[readOff];
		readOff++;
		writeOff += dir;
	}

	return offset+size;
}

const MemUtil memUtil = {malloc2D, free2D, fillBytes};
