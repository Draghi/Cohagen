//#define GL_GLEXT_PROTOTYPES

#include "VBO.h"
#include "../graphics.h"
#include <stdlib.h>
#include <stdbool.h>

static VBO* boundBuffer;

///////////////
// VBO Class //
///////////////
/**
 * Binds the vbo to the array buffer.
 *
 * @param self The pointer to the instance of the vbo.
 * @return If the bind was successful, or not.
 */
static bool bind(VBO* self) {
	if (boundBuffer!=self) {
		boundBuffer = self;
		glBindBuffer(GL_ARRAY_BUFFER, self->id);

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
static bool unbind(VBO* self) {
	if (boundBuffer==self) {
		boundBuffer = NULL;
		glBindBuffer(GL_ARRAY_BUFFER, self->id);

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
static bool setData(VBO* self, GLvoid* data, GLsizeiptr size, GLenum usage) {
	if (bind(self)) {
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		unbind(self);
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
static bool subData(VBO* self, GLvoid* data, GLsizeiptr size, GLintptr offset) {
	if (bind(self)) {
		glBufferSubData(self->id, offset, size, data);

		unbind(self);
		return true;
	} else {
		return false;
	}
}

///////////////////////
// VBO Manager Class //
///////////////////////
/**
 * Creates a new VBO.
 * @return A pointer to the new VBO.
 */
static VBO* createVBO(){
	VBO* vbo = malloc(sizeof(VBO));

	glGenBuffers(1, (&vbo->id));
	vbo->bind = bind;
	vbo->unbind = unbind;
	vbo->subData = subData;
	vbo->setData = setData;

	return vbo;
}

////////////////////////
// Singleton Instance //
////////////////////////
/**
 * The instance of the "fake object" singleton for the VBOManager.
 * Each element corresponds to the strut defined in the header, in order.
 * Do not, I repeat DO NOT mess with this object, unless you are certain about what you're doing.
 */
const VBOManager vboManager = {createVBO};
