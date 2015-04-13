#include "ShaderBuilder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gl/ShaderLoader.h"
#include "util/DynamicArray.h"
#include "util/Log.h"

static GLuint   compileShader(GLenum shaderType, GLubyte **shaderData);
static void     getFileName(char **filename, const char *const path, const char *const baseFileName, const char *const extension);
static GLuint   linkProgram(int numShaders, const DynamicArray *const shaderList);
static GLuint   loadShader(GLenum type, const char *const path, const char *const baseFileName);
static GLuint   loadShaders(const char *const path, const char *const baseFileName);

/*
 *  Compile given shader source of given shader type and return GL shader handle.
 *
 *  @param  shaderType  GLenum, OpenGL shader type (eg. GL_VERTEX_SHADER)
 *  @param  shaderData  pointer to pointer to GLubyte, shader source.
 *  @returns            GLuint, GL shader handle.
 */
static GLuint compileShader(GLenum shaderType, GLubyte **shaderData) {
    GLuint shader = 0;
    
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const GLchar **) shaderData, NULL);
    
    glCompileShader(shader);

    cohLog.logCompilationErrors(shader);
    
    return shader;
}

/**
 *  Gets a filename with the given path, filename and extension and puts it into 
 *  the first argument.
 *
 *  @param  filename        pointer to pointer to char, will contain full filename + path when function completes
 *                          successfully. Memory must be freed by user.
 *  @param  path            const pointer to const char, path to file.
 *  @param  baseFileName    const pointer to const char, filename.
 *  @param  extension       const pointer to const char, extension to add.
 */
static void getFileName(char **filename, const char *const path, const char *const baseFileName, const char *const extension) {
    int pathSize = strlen(path);
    int baseFileNameSize = strlen(baseFileName);
    int extensionSize = strlen(extension);
    *filename = malloc((pathSize + baseFileNameSize + extensionSize) * sizeof(char) + 1/**NULL terminator*/);

    strcpy(*filename, path);
    strcpy(*filename + pathSize, baseFileName);
    strcpy(*filename + pathSize + baseFileNameSize, extension);
}

/*
 *  Link a list of shaders into a program.
 *
 *  @param  numShaders  int, number of shaders to link.
 *  @param  shaderList  const pointer to const Dynamic array containing a list of compiled shader objects (GLuints).
 *  @returns            GLuint, linked shader program. 0 in case of error.
 */
static GLuint linkProgram(int numShaders, const DynamicArray *const shaderList) {
    GLuint      programObject = glCreateProgram();


    // Attach shaders 
    for (int i = 0; i < shaderList->size; ++i)
    {
        GLuint *shader = (GLuint *) manDynamicArray.get(shaderList, i);
        glAttachShader(programObject, *shader);
    }

    // Link
    glLinkProgram(programObject);

    cohLog.logLinkerErrors(programObject);

    // Clean up
    for (int i = 0; i < shaderList->size; ++i)
    {
        GLuint *shader = (GLuint *) manDynamicArray.get(shaderList, i);

        glDetachShader(programObject, *shader);
        glDeleteShader(*shader);
    }

    return programObject;
}

/*
 *  Load shader of given type at specified path, with given base file name and return
 *  a handle to a GL shader object.
 *  Shaders that should be included together must have the same base file name and end in
 *  ".vert" for vertex shader, ".frag" for fragment shader and ".geom" for geometry shader.
 * 
 *  @param  type    GLenum, OpenGL shader type.
 *  @param  path    const pointer to const char, path to shader.
 *  @param  baseFileName    const pointer to const char, base file name of shaders.
 *  @
 */
static GLuint loadShader(GLenum type, const char *const path, const char *const baseFileName) {
    GLuint  shader = 0;
    GLubyte *shaderSource = NULL;

    // Get filename
    char *filename = NULL;

    switch (type) {
        case GL_VERTEX_SHADER:
            getFileName(&filename, path, baseFileName, ".vert");
            break;
        case GL_FRAGMENT_SHADER:
            getFileName(&filename, path, baseFileName, ".frag");
            break;
        // case GL_GEOMETRY_SHADER:
        //     getFileName(&filename, path, baseFileName, ".geom");
        //     break;
        default:
            break;        
    }

    // Load shader
    shaderLoader.loadShaderString(filename, &shaderSource);

    // Compile vertex shader
    shader = compileShader(type, &shaderSource);

    free(filename);

    return shader;
}

static GLuint loadShaders(const char *const path, const char *const baseFileName) {
    // Number of types of shaders (vertex, frag, geo etc.)
    const int   numTypesShaders = 2;
    GLuint      shaders[numTypesShaders];
    GLuint      program = 0;

    // Load shaders
    shaders[0] = loadShader(GL_VERTEX_SHADER, path, baseFileName);
    shaders[1] = loadShader(GL_FRAGMENT_SHADER, path, baseFileName);

    DynamicArray *shaderList = manDynamicArray.new(8, sizeof(GLuint *));

    for (int i = 0; i < numTypesShaders; ++i) {
        GLuint *temp = (GLuint *) calloc(1, sizeof(GLuint *));
        *temp = shaders[i];
        manDynamicArray.append(shaderList, temp);
    }

    program = linkProgram(numTypesShaders, shaderList);

    // Clean up shaderList
    manDynamicArray.freeContents(shaderList);
    manDynamicArray.delete(shaderList);

    return program;
}

const ShaderBuilder shaderBuilder = {loadShaders};
