#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ShaderLoader.h"
#include "Log.h"

static GLuint compileShader(GLenum shaderType, GLubyte **shaderData);
static void logCompilationErrors(GLuint shader);
static void logShaderInfoLog(GLuint shader);
static void logLinkerErrors(GLuint program);
static void logProgramInfoLog(GLuint program, int (*logFunc)(const char* message, ...));
static void logShaderInformation(const Shader *const shader);
static const char  *toStringGLType(GLenum type);

// static GLuint shaderFromFile(GLenum type, const char *const filename) {
//     GLuint      shaderObject = 0;
//     GLubyte     *shaderSource;

//     FILE    *fp;
//     long    fileSize;

//     fp = fopen(filename, "r");

//     if (fp != NULL) {
//         // Navigate to end of file
//         fseek(fp, 0, SEEK_END);
//         // Get position indicator at end of file (ie. length)
//         fileSize = ftell(fp);
//         // Navigate to start of file
//         fseek(fp, 0, SEEK_SET);

//         // Allocate enough memory to hold file
//         shaderSource = (GLubyte *) calloc(1, fileSize + 1);
//         memset(shaderSource, 0, fileSize + 1);

//         // Read stream into array
//         fread(shaderSource, fileSize + 1, 1, fp);

//         // Compile shader
//         shaderObject = compileShader(type, &shaderSource);
//     } else {
//         cohLog.logGLError("ERROR: Could not open SHADER_FILE \"%s\" for reading", filename);
//     }

//     // Clean up
//     fclose(fp);

//     if (shaderSource != NULL) {
//         free(shaderSource);
//     }

//     return shaderObject;
// }

static void loadShaderString(const char *const filename, GLubyte **loadedShaderString) {
    GLuint      shaderObject = 0;
    GLubyte     *shaderSource;

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

    logLinkerErrors(programObject);

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

    logCompilationErrors(shader);
    
    return shader;
}

static void logCompilationErrors(GLuint shader) {
    int     params = -1;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
    
    if (params != GL_TRUE) {
        cohLog.logGLError("ERROR: GL shader index %i did not compile\n", shader);
        
        // Log additional info
        logShaderInfoLog(shader);
    }
}
    
static void logShaderInfoLog(GLuint shader) {
    const int   maxLength = 2048;
    int         actualLength = 0;
    char        log[maxLength];
    
    glGetShaderInfoLog(shader, maxLength, &actualLength, log);
    cohLog.logGLError("SHADER INFO LOG: Shader index %u:\n%s\n", shader, log);
}

static void logLinkerErrors(GLuint program) {
    int params = -1;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    
    if (params != GL_TRUE) {
        cohLog.logGLError("ERROR: GL shader program index %u could not be linked\n", program);
            
        // Log additionalInfo
        logProgramInfoLog(program, cohLog.logGLError);
    }   
}

static void logProgramInfoLog(GLuint program, int (*logFunc)(const char* message, ...)) {
    const int   maxLength = 2048;
    int         actualLength = 0;
    char        log[maxLength];

    glGetProgramInfoLog(program, maxLength, &actualLength, log);
    logFunc("\nPROGRAM INFO LOG: Program index %u:\n%s", program, log);
}

static void logShaderInformation(const Shader *const shader) {
    cohLog.logGL("\n----------------------------------------\n");
    cohLog.logGL("PROGRAM INFO: Program index %i:\n", shader->program);
    
    int params = -1;
    glGetProgramiv(shader->program, GL_LINK_STATUS, &params);
    cohLog.logGL(" GL_LINK_STATUS = %i\n", params);
    
    glGetProgramiv(shader->program, GL_ATTACHED_SHADERS, &params);
    cohLog.logGL(" GL_ATTACHED_SHADERS = %i\n", params);
    
    // Log active attributes
    glGetProgramiv(shader->program, GL_ACTIVE_ATTRIBUTES, &params);
    cohLog.logGL(" GL_ACTIVE_ATTRIBUTES = %i\n", params);
    for (GLuint i = 0; i < (GLuint)params; ++i) {
        const int   maxLength = 64;
        char        name[maxLength];
        int         actualLength = 0;
        int         size = 0;
        GLenum      type;
        
        glGetActiveAttrib(shader->program, i, maxLength, &actualLength, &size, &type, name);
        if (size > 1) {
            for (int j = 0; j < size; ++j) {
                char    longName[maxLength];
                sprintf(longName, "%s[%i]", name, j);
                int location = glGetAttribLocation(shader->program, longName);
                cohLog.logGL("  %i) type:%s name:%s location:%i\n", i, toStringGLType(type), longName, location);
            }
        } else {
            int location = glGetAttribLocation(shader->program, name);
            cohLog.logGL("  %i) type:%s name:%s location:%i\n", i, toStringGLType(type), name, location);
        }
    }
    
    // Log active uniforms
    glGetProgramiv(shader->program, GL_ACTIVE_UNIFORMS, &params);
    cohLog.logGL(" GL_ACTIVE_UNIFORMS = %i\n", params);
    for (GLuint i = 0; i < (GLuint) params; ++i) {
        const int   maxLength = 64;
        char        name[maxLength];
        int         actualLength = 0;
        int         size = 0;
        GLenum type;
    
        glGetActiveUniform(shader->program, i, maxLength, &actualLength, &size, &type, name);
        if (size > 1) {
            for (int j = 0; j < size; ++j) {
                char    longName[maxLength];
                sprintf(longName, "%s[%i]", name, j);
                int location = glGetUniformLocation(shader->program, longName);
                cohLog.logGL("  %i) type:%s name:%s location:%i\n", i, toStringGLType(type), longName, location);
            }
        } else {
            int location = glGetUniformLocation(shader->program, name);
            cohLog.logGL("  %i) type:%s name:%s location:%i\n", i, toStringGLType(type), name, location);
        }
    }
    
    logProgramInfoLog(shader->program, cohLog.logGL);
}

static const char  *toStringGLType(GLenum type) {
    switch  (type) {
        case GL_BOOL: return "bool";
        case GL_INT: return "int";
        case GL_FLOAT: return "float";
        case GL_FLOAT_VEC2: return "vec2";
        case GL_FLOAT_VEC3: return "vec3";
        case GL_FLOAT_VEC4: return "vec4";
        case GL_FLOAT_MAT2: return "mat2";
        case GL_FLOAT_MAT3: return "mat3";
        case GL_FLOAT_MAT4: return "mat4";
        case GL_SAMPLER_2D: return "sampler2D";
        case GL_SAMPLER_3D: return "sampler3D";
        case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
        default: break;
    }
    return "other";
}

const ShaderLoader shaderLoader = {loadShaderString, linkProgram, logShaderInformation};