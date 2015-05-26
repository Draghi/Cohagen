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

void bindUniformScalar(GLint location, const scalar scal){
	glUniform1f(location, scal);
}

void bindUniformVec2(GLint location, const Vec2 *const vec){
	glUniform2f(location, vec->x, vec->y);
}

void bindUniformVec3(GLint location, const Vec3 *const vec){
	glUniform3f(location, vec->x, vec->y, vec->z);
}

void bindUniformVec4(GLint location, const Vec4 *const vec){
	glUniform4f(location, vec->x, vec->y, vec->z, vec->w);
}

void bindUniformInt1(GLint location, GLint v0){
	glUniform1i(location, v0);
}

void bindUniformInt2(GLint location, GLint v0, GLint v1){
	glUniform2i(location, v0, v1);
}

void bindUniformInt3(GLint location, GLint v0, GLint v1, GLint v2){
	glUniform3i(location, v0, v1, v2);
}

void bindUniformInt4(GLint location, GLint v0, GLint v1, GLint v2, GLint v3){
	glUniform4i(location, v0, v1, v2, v3);
}

void bindUniformUInt1(GLint location, GLuint v0){
	glUniform1ui(location, v0);
}

void bindUniformUInt2(GLint location, GLuint v0, GLuint v1){
	glUniform2ui(location, v0, v1);
}

void bindUniformUInt3(GLint location, GLuint v0, GLuint v1, GLuint v2){
	glUniform3ui(location, v0, v1, v2);
}

void bindUniformUInt4(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3){
	glUniform4ui(location, v0, v1, v2, v3);
}

const ShaderManager manShader = {bind, unbind, newFromGroup, bindUniformMat4, bindUniformInt, bindUniformFloat};
