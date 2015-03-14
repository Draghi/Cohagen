#ifndef COH_SHADERLOADER_H
#define COH_SHADERLOADER_H

#include <GL/glew.h>

typedef struct ShaderLoader_s {
    /**
     *  Load a shader from a file.
     *
     *  @param  type        GLenum, type of shader (eg. GL_VERTEX_SHADER, GL_FRAGMENT_SHADER)
     *  @param  filename    const pointer to const char, path to shader source file.
     *  @returns            GLuint, handle to GL shader object.
     */
    GLuint (*shaderFromFile)(GLenum type, const char *const filename);
} ShaderLoader;

extern const ShaderLoader shaderLoader;

#endif