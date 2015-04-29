#ifndef COH_MEMUTIL_H
#define COH_MEMUTIL_H

typedef struct MemUtil_s {
	void** (* malloc2D)(uint64_t elementSize, uint64_t xCount, uint64_t yCount);
	void (* free2D)(void** array, uint64_t xCount);
} MemUtil;

extern const MemUtil memUtil;

#endif
