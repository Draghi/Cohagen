#ifndef COH_SHADER_H
#define COH_SHADER_H

#include <stdarg.h>

#include "../lib/ogl.h"

/**
 *  Class to manage the use of shader programs.
 */
typedef struct Shader_s {
    /**
     *  Bind the shader program.
     *
     *  @params     shader  pointer to Shader object to bind.
     */
    void (*bind)(const struct Shader_s *const);

    /**
     *  Unbind the shader program.
     */
    void (*unbind)();

    /**
     *  GL linked program object.
     */
    GLuint program;
} Shader;

/**
 *  Link given GL shaders and use them to fill out the given Shader object.
 *
 *  @params     shader      Shader object to fill out.
 *  @params     numShaders  Number of shaders to compile into Shader object.
 *  @params     ...         Variable number of GLuint, compiled shader objects to be linked.
 */
void createShader(Shader *const shader, int numShaders, ...);

#endif
