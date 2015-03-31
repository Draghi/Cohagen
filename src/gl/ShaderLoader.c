#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ShaderLoader.h"
#include "../util/Log.h"

static void loadShaderString(const char *const filename, GLubyte **loadedShaderString) {
    FILE    *fp;
    long    fileSize;

    fp = fopen(filename, "r");

    if (fp != NULL) {
        // Navigate to end of file
        fseek(fp, 0, SEEK_END);
        // Get position indicator at end of file (ie. length)
        fileSize = ftell(fp);
        // Navigate to start of file
        fseek(fp, 0, SEEK_SET);

        // Allocate enough memory to hold file
        *loadedShaderString = (GLubyte *) calloc(1, fileSize + 1);
        memset(*loadedShaderString, 0, fileSize + 1);

        // Read stream into array
        fread(*loadedShaderString, fileSize + 1, 1, fp);
    } else {
        cohLog.logGLError("ERROR: Could not open SHADER_FILE \"%s\" for reading", filename);
    }

    // Clean up
    fclose(fp);
}

const ShaderLoader shaderLoader = {loadShaderString};
