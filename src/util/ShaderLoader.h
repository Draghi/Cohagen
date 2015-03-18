#ifndef COH_SHADERLOADER_H
#define COH_SHADERLOADER_H

#include <stdarg.h>

#include "../lib/opengl/gl_compat_3_0.h"

#include "DynamicArray.h"
#include "Shader.h"

/**
 *  Singleton for loading shader source code.
 */
typedef struct ShaderLoader_s {
    /**
     *  Load a shader from a file.
     *
     *  @param  type        GLenum, type of shader (eg. GL_VERTEX_SHADER, GL_FRAGMENT_SHADER)
     *  @param  filename    const pointer to const char, path to shader source file.
     *  @returns            GLuint, handle to GL shader object.
     */
    GLuint (*shaderFromFile)(GLenum, const char *const);

    /**
     *  Link a list of shaders into a program.
     *
     *  @param  numShaders  int, number of shaders to link.
     *  @param  shaderList  const pointer to const Dynamic array containing a list of compiled shader objects (GLuints).
     *  @returns            GLuint, linked shader program. 0 in case of error.
     */
    GLuint (*linkProgram)(int, const DynamicArray *const);

    /**
     *  Write detailed information about a shader to the log.
     *
     *  @param  shader  const pointer to const Shader to log information about.
     */
    void (*logShaderInformation)(const Shader *const shader);
} ShaderLoader;

extern const ShaderLoader shaderLoader;

#endif
