#ifndef COH_LOG_H
#define COH_LOG_H

#include <stdarg.h>

#include "gl/Shader.h"

#define GL_LOG_FILE "./gl.log"

/**
 *  Log used to track issues with
 *  shader compilation and linking.
 */
typedef struct Log_s {
    /**
     *  Initialize the log. Required to call this function
     *  before any other log functions are called.
     *
     *  @returns    int, 0 if unsuccessful, 1 if successful.
     */
    int (*logGLRestart)();

    /**
     *  Write a message to the GL log.
     *
     *  @param  message     pointer to const char, the message to write.
     *  @param  ...         Variable number of arguments to print.
     *  @returns            int, 0 if write is unsuccessfult, 1 if it is successful.
     */
    int (*logGL)(const char *message, ...);

    /**
     *  Write a message to the GL log and stderr.
     *
     *  @param  message     pointer to const char, the message to write.
     *  @param  ...         Variable number of arguments to print.
     *  @returns            int, 0 if write is unsuccessfult, 1 if it is successful.
     */
    int (*logGLError)(const char *message, ...);

    /**
     *  Log any compilation errors with the given shader.
     *
     *  @param  shader  GLuint, GL handle to shader to check.
     */
    void (*logCompilationErrors)(GLuint shader);

    /**
     *  Log any linker errors with the given program.
     *
     *  @param  program     GLuint, GL handle to program to check.
     */
    void (*logLinkerErrors)(GLuint program);

    /**
     *  Log detailed information about the given shader object.
     */
    void (*logShaderInformation)(const Shader *const shader);
} Log;

extern const Log cohLog;

#endif
