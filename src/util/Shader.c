#include <stdlib.h>

#include "Shader.h"
#include "ShaderLoader.h"
#include "DynamicArray.h"

static void bind(const Shader *const shader);
static void unbind();

void createShader(Shader *const shader, int numShaders, ...) {
    shader->bind = bind;
    shader->unbind = unbind;

    // Hand-off argument list to another function.
    DynamicArray shaderList;
    newDynamicArray(&shaderList, 8, sizeof(GLuint));

    va_list argptr;
    va_start(argptr, numShaders);

    for (int i = 0; i < numShaders; ++i) {
        GLuint *temp = (GLuint *) calloc(1, sizeof(GLuint));
        *temp = va_arg(argptr, GLuint);
        shaderList.append(&shaderList, temp);
    }

    va_end(argptr);

    shader->program = shaderLoader.linkProgram(numShaders, &shaderList);

    // Clean up shaderList
    freeDynamicArrayContents(&shaderList);
    deleteDynamicArray(&shaderList);
}

static void bind(const Shader *const shader) {
    glUseProgram(shader->program);
}

static void unbind() {
    glUseProgram(0);
}
