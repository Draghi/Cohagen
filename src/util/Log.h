#ifndef COH_LOG_H
#define COH_LOG_H

#include <stdarg.h>

#define GL_LOG_FILE "./gl.log"

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
} Log;

extern const Log glLog;

#endif