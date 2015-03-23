#include <time.h>
#include <stdio.h>

#include "Log.h"

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

const Log cohLog = {logGLRestart, logGL, logGLError};