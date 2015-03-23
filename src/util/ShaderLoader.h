#ifndef COH_SHADERLOADER_H
#define COH_SHADERLOADER_H

#include <stdarg.h>

#include "graphics.h"

/**
 *  Singleton for loading shader source code.
 */
typedef struct ShaderLoader_s {
    /**
     *  Load a shader from a file, storing it in a GLubyte array passed in by the user.
     *
     *  @param  filename            const pointer to const char, path to shader source file.
     *  @param  loadedShaderString  pointer to pointer to GLubyte, will contain shader string after function is called.
     *  @returns                    GLuint, handle to GL shader object.
     */
    void (*loadShaderString)(const char *const filename, GLubyte **loadedShaderString);
} ShaderLoader;

extern const ShaderLoader shaderLoader;

#endif
