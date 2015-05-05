#include "EAB.h"

#include <stdlib.h>
#include <stdbool.h>

#include "lib/ogl.h"

///////////////////////
// EAB Manager Class //
///////////////////////
/**
 * Creates a new EAB.
 * @return A pointer to the new EAB.
 */
static EAB* new(){
	EAB* EAB = malloc(sizeof(EAB));

	glGenBuffers(1, (&EAB->id));
	EAB->countPerVert = 0;
	EAB->pointer = NULL;
	EAB->stride = 0;
	EAB->vertCount = 0;

	return EAB;
}

/**
 * Binds the EAB to the array buffer.
 *
 * @param EAB The EAB to bind.
 * @return If the bind was successful, or not.
 */
static bool bind(const EAB* const EAB) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB->id);
	return true;
}

/**
 * Unbinds the EAB from the array buffer.
 *
 * @param EAB The EAB to unbind.
 * @return If the unbind was successful, or not. Normally, false means it wasn't bound.
 */
static bool unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}

/**
 * Creates or changes the data that the EAB references.
 *
 * @param EAB The EAB to alter.
 * @param data A pointer to the data to bind.
 * @param size The size in bytes of the data.
 * @param usage The OpenGL usage hint (GL_STATIC_DRAW eg.)
 * @return Whether the data was set or not.
 */
static bool setData(const EAB* const EAB, GLvoid* data, GLsizeiptr size, GLenum usage) {
	if (bind(EAB)) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
		unbind(EAB);
		return true;
	} else {
		return false;
	}
}

/**
 * Substitutes part of the data that the EAB references with the given data/
 *
 * @param EAB The EAB to alter.
 * @param data A pointer to the data to bind.
 * @param size The size in bytes to add from the data.
 * @param offset The offset into the EAB's data to start writing at, in bytes.
 * @return Whether the data was substituted or not.
 */
static bool subData(const EAB* const EAB, GLvoid* data, GLsizeiptr size, GLintptr offset) {
	if (bind(EAB)) {
		glBufferSubData(EAB->id, offset, size, data);
		unbind();
		return true;
	} else {
		return false;
	}
}

/**
 * Sets the number of vertices and the number of elements per vertex for the EAB.
 *
 * @param EAB The EAB to alter.
 * @param vertCount The number of vertices the EAB represents.
 * @param countPerVert The number of elements that represents a vertex.
 * @param stride Number of bytes from the start of one element to the start of the next.
 * @param pointer Offset to start reading from.
 */
static void setRenderInfo(EAB* const EAB, GLint vertCount, GLint countPerVert, GLint stride, GLvoid *pointer) {
	EAB->vertCount=vertCount;
	EAB->countPerVert=countPerVert;
	EAB->stride = stride;
	EAB->pointer = pointer;
}

/**
 * Frees the buffer from both the Computer's and the GPU's memory.
 * @param self The pointer to the instance of the EAB.
 */
static void delete(EAB* const EAB) {
	glDeleteBuffers(1, &(EAB->id));
	free(EAB);
}

////////////////////////
// Singleton Instance //
////////////////////////
/**
 * The instance of the "fake object" singleton for the EABManager.
 * Each element corresponds to the strut defined in the header, in order.
 * Do not, I repeat DO NOT mess with this object, unless you are certain about what you're doing.
 */
const EABManager manEAB = {new, bind, unbind, setData, subData, setRenderInfo, delete};
