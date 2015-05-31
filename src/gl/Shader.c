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
		scalar *data = malloc(16 * sizeof(scalar));
		manMat4.getMat4Data(matrix, data);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, data);

		free(data);
	}

	return uniformLocation;
}

static int bindUniformInt(const Shader *const shader, const char *uniformName, int intToBind) {
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform1i(uniformLocation, intToBind);
	}

	return uniformLocation;
}

static int bindUniformFloat(const Shader *const shader, const char *uniformName, float floatToBind) {
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform1f(uniformLocation, floatToBind);
	}

	return uniformLocation;
}

void bindUniformVec2(const Shader *const shader, const char *uniformName, const Vec2 *const vec){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform2f(uniformLocation, vec->x, vec->y);
	}

}

void bindUniformVec3(const Shader *const shader, const char *uniformName, const Vec3 *const vec){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform3f(uniformLocation, vec->x, vec->y, vec->z);
	}

}

void bindUniformVec4(const Shader *const shader, const char *uniformName, const Vec4 *const vec){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform4f(uniformLocation, vec->x, vec->y, vec->z, vec->w);
	}

}

void bindUniformInt2(const Shader *const shader, const char *uniformName, GLint v0, GLint v1){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform2i(uniformLocation, v0, v1);
	}

}

void bindUniformInt3(const Shader *const shader, const char *uniformName, GLint v0, GLint v1, GLint v2){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform3i(uniformLocation, v0, v1, v2);
	}

}

void bindUniformInt4(const Shader *const shader, const char *uniformName, GLint v0, GLint v1, GLint v2, GLint v3){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform4i(uniformLocation, v0, v1, v2, v3);
	}

}

void bindUniformUInt1(const Shader *const shader, const char *uniformName, GLuint v0){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform1ui(uniformLocation, v0);
	}

}

void bindUniformUInt2(const Shader *const shader, const char *uniformName, GLuint v0, GLuint v1){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform2ui(uniformLocation, v0, v1);
	}

}

void bindUniformUInt3(const Shader *const shader, const char *uniformName, GLuint v0, GLuint v1, GLuint v2){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform3ui(uniformLocation, v0, v1, v2);
	}

}

void bindUniformUInt4(const Shader *const shader, const char *uniformName, GLuint v0, GLuint v1, GLuint v2, GLuint v3){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform4ui(uniformLocation, v0, v1, v2, v3);
	}

}

static int getUniformLocation(const Shader *const shader, const char *uniformName) {
	return glGetUniformLocation(shader->program, uniformName);
}

static int getAttribLocation(const Shader *const shader, const char *attribName) {
	return glGetAttribLocation(shader->program, attribName);
}

const ShaderManager manShader = {bind, unbind, newFromGroup, bindUniformMat4, bindUniformInt, bindUniformFloat, bindUniformVec2, bindUniformVec3, bindUniformVec4, bindUniformInt2, bindUniformInt3, bindUniformInt4, bindUniformUInt1, bindUniformUInt2, bindUniformUInt3, bindUniformUInt4, getUniformLocation, getAttribLocation};
