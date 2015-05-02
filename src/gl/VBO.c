#include "VBO.h"

#include <stdlib.h>
#include <stdbool.h>

#include "lib/ogl.h"

///////////////////////
// VBO Manager Class //
///////////////////////
/**
 * Creates a new VBO.
 * @return A pointer to the new VBO.
 */
static VBO* new(){
	VBO* vbo = malloc(sizeof(VBO));

	glGenBuffers(1, (&vbo->id));
	vbo->countPerVert = 0;
	vbo->pointer = NULL;
	vbo->stride = 0;
	vbo->vertCount = 0;

	return vbo;
}

/**
 * Binds the vbo to the array buffer.
 *
 * @param vbo The VBO to bind.
 * @return If the bind was successful, or not.
 */
static bool bind(const VBO* const vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
	return true;
}

/**
 * Unbinds the vbo from the array buffer.
 *
 * @param vbo The VBO to unbind.
 * @return If the unbind was successful, or not. Normally, false means it wasn't bound.
 */
static bool unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

/**
 * Creates or changes the data that the VBO references.
 *
 * @param vbo The VBO to alter.
 * @param data A pointer to the data to bind.
 * @param size The size in bytes of the data.
 * @param usage The OpenGL usage hint (GL_STATIC_DRAW eg.)
 * @return Whether the data was set or not.
 */
static bool setData(const VBO* const vbo, GLvoid* data, GLsizeiptr size, GLenum usage) {
	if (bind(vbo)) {
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		unbind(vbo);
		return true;
	} else {
		return false;
	}
}

/**
 * Substitutes part of the data that the VBO references with the given data/
 *
 * @param vbo The VBO to alter.
 * @param data A pointer to the data to bind.
 * @param size The size in bytes to add from the data.
 * @param offset The offset into the VBO's data to start writing at, in bytes.
 * @return Whether the data was substituted or not.
 */
static bool subData(const VBO* const vbo, GLvoid* data, GLsizeiptr size, GLintptr offset) {
	if (bind(vbo)) {
		glBufferSubData(vbo->id, offset, size, data);
		unbind();
		return true;
	} else {
		return false;
	}
}

/**
 * Sets the number of vertices and the number of elements per vertex for the VBO.
 *
 * @param vbo The VBO to alter.
 * @param vertCount The number of vertices the VBO represents.
 * @param countPerVert The number of elements that represents a vertex.
 * @param stride Number of bytes from the start of one element to the start of the next.
 * @param pointer Offset to start reading from.
 */
static void setRenderInfo(VBO* const vbo, GLint vertCount, GLint countPerVert, GLint stride, GLvoid *pointer) {
	vbo->vertCount=vertCount;
	vbo->countPerVert=countPerVert;
	vbo->stride = stride;
	vbo->pointer = pointer;
}

/**
 * Frees the buffer from both the Computer's and the GPU's memory.
 * @param self The pointer to the instance of the vbo.
 */
static void delete(VBO* const vbo) {
	glDeleteBuffers(1, &(vbo->id));
	free(vbo);
}

////////////////////////
// Singleton Instance //
////////////////////////
/**
 * The instance of the "fake object" singleton for the VBOManager.
 * Each element corresponds to the strut defined in the header, in order.
 * Do not, I repeat DO NOT mess with this object, unless you are certain about what you're doing.
 */
const VBOManager manVBO = {new, bind, unbind, setData, subData, setRenderInfo, delete};
