//#define GL_GLEXT_PROTOTYPES

#include "VBO.h"
#include "../graphics.h"
#include <stdlib.h>
#include <stdbool.h>

static VBO* boundBuffer;

///////////////
// VBO Class //
///////////////
static bool bind(VBO* self) {
	if (boundBuffer!=self) {
		boundBuffer = self;
		glBindBuffer(GL_ARRAY_BUFFER, self->id);

		return true;
	} else {
		return false;
	}
}

static bool unbind(VBO* self) {
	if (boundBuffer==self) {
		boundBuffer = NULL;
		glBindBuffer(GL_ARRAY_BUFFER, self->id);

		return true;
	} else {
		return false;
	}
}

static bool setData(VBO* self, GLvoid* data, GLsizeiptr size, GLenum usage) {
	if (bind(self)) {
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		unbind(self);
		return true;
	} else {
		return false;
	}
}

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

static VBO* createVBO(){
	VBO* vbo = malloc(sizeof(VBO));

	glGenBuffers(1, (&vbo->id));
	vbo->bind = bind;
	vbo->unbind = unbind;
	vbo->subData = subData;
	vbo->setData = setData;

	return vbo;
}

const VBOManager vboManager = {createVBO};
