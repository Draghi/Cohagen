#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ShaderLoader.h"

static GLuint compileShader(GLenum shaderType, GLubyte **shaderData);

static GLuint shaderFromFile(GLenum type, const char *const filename) {
    GLuint      shaderObject = 0;
    GLubyte     *shaderSource;
    GLubyte     **shaderSourceHandle  = &shaderSource;

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
        shaderSource = (GLubyte *) calloc(1, fileSize + 1);
        *shaderSourceHandle = shaderSource;
        memset(*shaderSourceHandle, 0, fileSize + 1);

        // Read stream into array
        fread(*shaderSourceHandle, fileSize + 1, 1, fp);

        // Compile shader
        shaderObject = compileShader(type, shaderSourceHandle);
    }

    // Clean up
    fclose(fp);

    if (shaderSource != NULL) {
        free(shaderSource);
    }

    return shaderObject;
}

static GLuint linkProgram(int numShaders, const DynamicArray *const shaderList) {
    GLuint      programObject = glCreateProgram();


    // Attach shaders 
    for (int i = 0; i < shaderList->size; ++i)
    {
        GLuint *shader = (GLuint *) shaderList->get(shaderList, i);
        glAttachShader(programObject, *shader);
    }

    // Link
    glLinkProgram(programObject);

    // Clean up
    for (int i = 0; i < shaderList->size; ++i)
    {
        GLuint *shader = (GLuint *) shaderList->get(shaderList, i);

        glDetachShader(programObject, *shader);
        glDeleteShader(*shader);
    }

    return programObject;
}

static GLuint compileShader(GLenum shaderType, GLubyte **shaderData) {
    GLuint shader = 0;
    
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const GLchar **) shaderData, NULL);
    
    glCompileShader(shader);
    
    return shader;
}

const ShaderLoader shaderLoader = {shaderFromFile, linkProgram};