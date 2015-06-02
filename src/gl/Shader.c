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

static void bindUniformVec2(const Shader *const shader, const char *uniformName, const Vec2 *const vec){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform2f(uniformLocation, vec->x, vec->y);
	}

}

static void bindUniformVec3(const Shader *const shader, const char *uniformName, const Vec3 *const vec){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform3f(uniformLocation, vec->x, vec->y, vec->z);
	}

}

static void bindUniformVec4(const Shader *const shader, const char *uniformName, const Vec4 *const vec){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform4f(uniformLocation, vec->x, vec->y, vec->z, vec->w);
	}

}

static void bindUniformInt2(const Shader *const shader, const char *uniformName, GLint v0, GLint v1){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform2i(uniformLocation, v0, v1);
	}

}

static void bindUniformInt3(const Shader *const shader, const char *uniformName, GLint v0, GLint v1, GLint v2){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform3i(uniformLocation, v0, v1, v2);
	}

}

static void bindUniformInt4(const Shader *const shader, const char *uniformName, GLint v0, GLint v1, GLint v2, GLint v3){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform4i(uniformLocation, v0, v1, v2, v3);
	}

}

static void bindUniformUInt1(const Shader *const shader, const char *uniformName, GLuint v0){
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

static void bindUniformUInt3(const Shader *const shader, const char *uniformName, GLuint v0, GLuint v1, GLuint v2){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform3ui(uniformLocation, v0, v1, v2);
	}

}

static void bindUniformUInt4(const Shader *const shader, const char *uniformName, GLuint v0, GLuint v1, GLuint v2, GLuint v3){
	// Get uniform location
	GLuint uniformLocation = glGetUniformLocation(shader->program, uniformName);

	if (uniformLocation != -1) {
		glUniform4ui(uniformLocation, v0, v1, v2, v3);
	}

}

static GLuint genUniformBuffer(ShaderManager *const shaderManager, unsigned int size, GLuint *uniformBindingPoint) {
	GLuint uniformBuffer;

	// Generate uniform buffer object
	glGenBuffers(1, &uniformBuffer);

	// Make buffer appropriate size
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Bind uniform buffer object to binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, shaderManager->nextBindingPoint, uniformBuffer, 0, size);

	// Advance next binding point
	++(shaderManager->nextBindingPoint);

	return uniformBuffer;
}

static void bindUniformBlockProgram(const Shader *const shader, const char *uniformBlockName, GLuint uniformBindingPoint) {
	// Get uniform block location
	GLuint uniformBlockLocation = glGetUniformBlockIndex(shader->program, uniformBlockName);

	// Associate uniform block in this program with uniform buffer object @ binding point
	glUniformBlockBinding(shader->program, uniformBlockLocation, uniformBindingPoint);
}

static void bindUniformBufferSubData(GLuint uniformBufferObject, int startOffset, int size, const void *data) {
	// Bind data to buffer
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, startOffset, size, data);

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

static int getUniformLocation(const Shader *const shader, const char *uniformName) {
	return glGetUniformLocation(shader->program, uniformName);
}

static int getAttribLocation(const Shader *const shader, const char *attribName) {
	return glGetAttribLocation(shader->program, attribName);
}

// const ShaderManager manShader = {bind, unbind, newFromGroup, bindUniformMat4, bindUniformInt, bindUniformFloat, bindUniformVec2, bindUniformVec3, bindUniformVec4, bindUniformInt2, bindUniformInt3, bindUniformInt4, bindUniformUInt1, bindUniformUInt2, bindUniformUInt3, bindUniformUInt4, genUniformBuffer, bindUniformBlockProgram, bindUniformBufferSubData, getUniformLocation, getAttribLocation};

const ShaderManager manShader = {1, bind, unbind, newFromGroup, bindUniformMat4, bindUniformInt, bindUniformFloat, bindUniformVec2, bindUniformVec3, bindUniformVec4, bindUniformInt2, bindUniformInt3, bindUniformInt4, bindUniformUInt1, bindUniformUInt2, bindUniformUInt3, bindUniformUInt4, genUniformBuffer, bindUniformBlockProgram, bindUniformBufferSubData, getUniformLocation, getAttribLocation};




