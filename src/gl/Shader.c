#include "Shader.h"

#include <stdlib.h>
#include <stdio.h>

#include "gl/ShaderBuilder.h"
#include "util/DynamicArray.h"

static void bind(const Shader *const shader);
static void unbind();
static Shader *newFromGroup(const char *const path, const char *const baseFileName);
static int bindUniformMat4(const Shader *const shader, const char *uniformName, const Mat4 *const matrix);

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

static int bindUniformMat4(const Shader *const shader, const char *uniformName, const Mat4 *const matrix) {
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUseProgram(shader->program);
			scalar *data = malloc(16 * sizeof(scalar));
			manMat4.getMat4Data(matrix, data);
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, data);
		glUseProgram(0);

		free(data);
	}	
	return 1;
}

static int bindUniformInt(const Shader *const shader, const char *uniformName, int intToBind) {
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUseProgram(shader->program);
			glUniform1i(uniformLocation, intToBind);
		glUseProgram(0);
	}

	return uniformLocation;
}

static int bindUniformFloat(const Shader *const shader, const char *uniformName, float floatToBind) {
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUseProgram(shader->program);
			glUniform1f(uniformLocation, floatToBind);
		glUseProgram(0);
	}

	return uniformLocation;
}


const ShaderManager manShader = {bind, unbind, newFromGroup, bindUniformMat4, bindUniformInt, bindUniformFloat};
