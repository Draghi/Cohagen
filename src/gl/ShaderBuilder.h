#ifndef COH_SHADERBUILDER_H
#define COH_SHADERBUILDER_H

#include "ShaderLoader.h"

typedef struct ShaderBuilder_s {
    /**
     *  Searches for vertex, fragment and geometry shaders with the baseFileName
     *  in the given path and returns a handle to a program made from those shaders.
     *
     *  @param  path            const pointer to const char, path to search.
     *  @param  baseFileName    const pointer to const char, filename all the
     *      vertex, fragment, etc. shader source files share in common.
     *  @returns                GLuint, handle to GL program object.
     */
    GLuint (*loadShaders)(const char *const path, const char *const baseFileName);
} ShaderBuilder;

extern const ShaderBuilder shaderBuilder;

#endif
