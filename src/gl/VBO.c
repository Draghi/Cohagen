//#define GL_GLEXT_PROTOTYPES

#include "../graphics.h"
#include <stdlib.h>
#include <stdbool.h>

static VBO* boundBuffer;

///////////////////////
// VBO Manager Class //
///////////////////////
/**
 * Creates a new VBO.
 * @return A pointer to the new VBO.
 */
static VBO* newVBO(){
	VBO* vbo = malloc(sizeof(VBO));

	glGenBuffers(1, (&vbo->id));

	return vbo;
}/**
 * Binds the vbo to the array buffer.
 *
 * @param self The pointer to the instance of the vbo.
 * @return If the bind was successful, or not.
 */
static bool bind(VBO* vbo) {
	if (boundBuffer!=vbo) {
		boundBuffer = vbo;
		glBindBuffer(GL_ARRAY_BUFFER, vbo->id);

		return true;
	} else {
		return false;
	}
}

/**
 * Unbinds the vbo from the array buffer.
 *
 * @param self The pointer to the instance of the vbo.
 * @return If the unbind was successful, or not. Normally, false means it wasn't bound.
 */
static bool unbind(VBO* vbo) {
	if (boundBuffer==vbo) {
		boundBuffer = NULL;
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	} else {
		return false;
	}
}

/**
 * Creates or changes the data that the VBO references.
 *
 * @param self The pointer to the instance of the vbo.
 * @param data A pointer to the data to bind.
 * @param size The size in bytes of the data.
 * @param usage The OpenGL usage hint (GL_STATIC_DRAW eg.)
 * @return Whether the data was set or not.
 */
static bool setData(VBO* vbo, GLvoid* data, GLsizeiptr size, GLenum usage) {
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
 * @param self The pointer to the instance of the vbo.
 * @param data A pointer to the data to bind.
 * @param size The size in bytes to add from the data.
 * @param offset The offset into the VBO's data to start writing at, in bytes.
 * @return Whether the data was substituted or not.
 */
static bool subData(VBO* vbo, GLvoid* data, GLsizeiptr size, GLintptr offset) {
	if (bind(vbo)) {
		glBufferSubData(vbo->id, offset, size, data);

		unbind(vbo);
		return true;
	} else {
		return false;
	}
}

/**
 * Frees the buffer from both the Computer's and the GPU's memory.
 * @param self The pointer to the instance of the vbo.
 */
static void delete(VBO* vbo) {
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
const VBOManager vboManager = {newVBO, bind, unbind, setData, subData, delete};
