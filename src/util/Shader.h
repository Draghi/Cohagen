#ifndef COH_SHADER_H
#define COH_SHADER_H

#include <stdarg.h>

#include "graphics.h"

/**
 *  Shader object.
 */
typedef struct Shader_s {
    /**
     *  GL linked program object.
     */
    GLuint program;
} Shader;

/**
 *  Class to manage the use of Shader objects.
 */
typedef struct ShaderManager_s {
    /**
     *  Bind the shader program.
     *
     *  @params     shader  pointer to Shader object to bind.
     */
    void (*bind)(const Shader *const);

    /**
     *  Unbind the shader program.
     */
    void (*unbind)();

    /**
     *  Returns a pointer to a Shader object, constructed from all shaders
     *  matching the given baseFileName at the given path.
     *
     *  @param  path            const pointer to const char, path to shaders.
     *  @param  baseFileName    const pointer to const char, base file name of shaders.
     *  @returns                pointer to Shader object.
     */
    Shader *(*newFromGroup)(const char *const path, const char *const baseFileName);
} ShaderManager;

extern const ShaderManager shaderManager;

#endif
