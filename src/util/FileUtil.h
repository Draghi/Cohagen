#ifndef COH_FILELOADER_H
#define COH_FILELOADER_H

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct {
	off_t size;
	uint8_t *data;
} FileData;

typedef struct FileUtil_s {
	uint32_t (* getFileSize)(const char *filename, off_t* size);
	uint32_t (* loadFile)(const char *filename, FileData* dest);
} FileUtil;

extern const uint32_t FILE_SUCCEEDED;
extern const uint32_t FILE_FAIL;
extern const uint32_t FILE_FAIL_OPEN;
extern const uint32_t FILE_FAIL_CLOSE;

extern const FileUtil fileUtil;

#endif
