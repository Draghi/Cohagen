#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <util/FileUtil.h>

#if defined _WIN32 || defined _WIN64
	#include <sys/stat.h>
#elif defined __APPLE__
    #include "TargetConditionals.h"
	#if TARGET_OS_IPHONE
		#error Unsupported Plaform.
    #elif TARGET_OS_MAC
		#include <sys/stat.h>
	#else
		#error Unsupported Plaform.
	#endif
#elif defined __linux__
	#include <sys/stat.h>
#else
	#error Unsupported Plaform.
#endif

const uint32_t FILE_SUCCEEDED = 0x0000;
const uint32_t FILE_FAIL = 0x0001;
const uint32_t FILE_FAIL_OPEN = 0x0002;
const uint32_t FILE_FAIL_CLOSE = 0x0004;

static uint32_t oFile(const char *filename, const char* mode, FILE** file) {
	FILE* fi = fopen(filename, mode);

	if (fi!=NULL) {
		*file = fi;
		return FILE_SUCCEEDED;
	} else {
		return FILE_FAIL_OPEN;
	}
}

static uint32_t cFile(FILE* file) {
	if (fclose(file)==0) {
		return FILE_SUCCEEDED;
	} else {
		return FILE_FAIL_CLOSE;
	}
}

static uint32_t rFile(FILE* file, FileData* dest) {
	off_t in = fread(dest->data, sizeof(char), dest->size, file);

	if (in==dest->size)
		return FILE_SUCCEEDED;
	else
		return FILE_FAIL;
}

/**
 * @todo Add support for mac osx
 * @todo Test on windows
 **/
uint32_t getFileSize(const char *filename, off_t* size) {

	#if defined _WIN32 || defined _WIN64
		struct _stat st;

		if (_stat(filename, &st) == 0) {
			*size = st.st_size;
			return FILE_SUCCEEDED;
		} else {
			return FILE_FAIL;
		}
	#elif defined __APPLE__
		struct stat st;

		if (stat(filename, &st) == 0) {
			*size = st.st_size;
			return FILE_SUCCEEDED;
		} else {
			return FILE_FAIL;
		}
	#elif defined __linux__
		struct stat st;

		if (stat(filename, &st) == 0) {
			*size = st.st_size;
			return FILE_SUCCEEDED;
		} else {
			return FILE_FAIL;
		}
	#endif
}

uint32_t loadFile(const char *filename, FileData* dest) {
	FileData res;

	FILE* file = NULL;
	uint32_t err = oFile(filename, "rb", &file);

	if (err==FILE_SUCCEEDED) {
		err |= getFileSize(filename, &res.size);

		if (err==FILE_SUCCEEDED) {
			if (res.size>0) {
				res.data = malloc(res.size*sizeof(uint8_t));
				err |= rFile(file, &res);
			}

			err |= cFile(file);

			if (err==FILE_SUCCEEDED) {
				*dest = res;
			} else {
				if (res.size>0)
					free(res.data);
			}
		}
	}

	return err;
}

const FileUtil fileUtil = {getFileSize, loadFile};
