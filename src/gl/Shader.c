#include "Shader.h"

#include <stdlib.h>
#include <stdio.h>

#include "gl/ShaderBuilder.h"
#include "util/DynamicArray.h"

static void bind(const Shader *const shader);
static void unbind();
static Shader *newFromGroup(const char *const path, const char *const baseFileName);
static void bindUniformMat4(const Shader *const shader, const char *uniformName, const Mat4 *const matrix);

// void createShader(Shader *const shader, int numShaders, ...) {
//     shader->bind = bind;
//     shader->unbind = unbind;

//     // Hand-off argument list to another function.
//     DynamicArray shaderList;
//     newDynamicArray(&shaderList, 8, sizeof(GLuint));

//     va_list argptr;
//     va_start(argptr, numShaders);

//     for (int i = 0; i < numShaders; ++i) {
//         GLuint *temp = (GLuint *) calloc(1, sizeof(GLuint));
//         *temp = va_arg(argptr, GLuint);
//         shaderList.append(&shaderList, temp);
//     }

//     va_end(argptr);

//     //shader->program = shaderBuilder.linkProgram(numShaders, &shaderList);

//     // Clean up shaderList
//     freeDynamicArrayContents(&shaderList);
//     deleteDynamicArray(&shaderList);
// }

static void bind(const Shader *const shader) {
    glUseProgram(shader->program);
}

static void unbind() {
    glUseProgram(0);
}

static Shader *newFromGroup(const char *const path, const char *const baseFileName) {
    Shader *newShader = (Shader *) calloc(1, sizeof(Shader));

    newShader->program = shaderBuilder.loadShaders(path, baseFileName);

    return newShader;
}

static void bindUniformMat4(const Shader *const shader, const char *uniformName, const Mat4 *const matrix)
{
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	// Get matrix data
	scalar *data = malloc(16 * sizeof(scalar));
	manMat4.getMat4Data(matrix, data);

	glUseProgram(shader->program);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, data);
	glUseProgram(0);

	free(data);
}

const ShaderManager manShader = {bind, unbind, newFromGroup, bindUniformMat4};
