#include <time.h>
#include <stdio.h>

#include "Log.h"

static void logCompilationErrors(GLuint shader);
static void logShaderInfoLog(GLuint shader);
static void logLinkerErrors(GLuint program);
static void logProgramInfoLog(GLuint program, int (*logFunc)(const char* message, ...));
static void logShaderInformation(const Shader *const shader);
static const char  *toStringGLType(GLenum type);

static int logGLRestart() {
    FILE    *file = fopen(GL_LOG_FILE, "w");
    int    couldOpen = 0;

    if (file) {
        couldOpen = 1;

        time_t now = time(NULL);
        char *date = ctime(&now);
        fprintf(file, "GL_LOG_FILE log. Local time %s\n", date);

        fclose(file);
    } else {
        fprintf(stderr, "ERROR: could not open GL_LOG_FILE log life %s for writing\n", GL_LOG_FILE);
    }

    return couldOpen;
}

static int logGL(const char *message, ...) {
    va_list     argptr;
    FILE        *file = fopen(GL_LOG_FILE, "a");
    int         couldOpen = 0;

    if (file) {
        couldOpen = 1;
        
        va_start(argptr, message);
        vfprintf(file, message, argptr);
        va_end(argptr);

        fclose(file);
    } else {
        fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n", GL_LOG_FILE);
    }
    
    return couldOpen;
}

static int logGLError(const char* message, ...) {
    va_list     argptr;
    FILE*       file = fopen(GL_LOG_FILE, "a");
    int         couldOpen = 0;  

    if (file) {
        couldOpen = 1;
        
        va_start(argptr, message);
        fprintf(file, "ERROR: ");
        vfprintf(file, message, argptr);
        fprintf(file, "\n");
        va_end(argptr);
        va_start(argptr, message);
        fprintf(stderr, "ERROR: ");
        vfprintf(stderr, message, argptr);
        fprintf(stderr, "\n");
        va_end(argptr);

        fclose(file);
    } else {
        fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n", GL_LOG_FILE);
    }
    
    return couldOpen;
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

/*
 *  Write detailed information about a shader to the log.
 *
 *  @param  shader  const pointer to const Shader to log information about.
 */
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

const Log cohLog = {logGLRestart, logGL, logGLError, logCompilationErrors, logLinkerErrors, logShaderInformation};