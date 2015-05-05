#ifndef COH_VAO_H
#define COH_VAO_H

#include <stdlib.h>
#include <stdbool.h>

#include "lib/ogl.h"
#include "gl/VBO.h"
#include "gl/EAB.h"
#include "util/DynamicArray.h"


struct VAO_s {
	GLuint id;
	uint32_t vertCount;
};

typedef struct VAO_s VAO;

struct VAOManager_s {
	/**
	 * Creates a new VAO.
	 * @return A new VAO
	 */
	VAO* (* new)();

	/**
	 * Binds the vao.
	 *
	 * @param vao The vao to bind.
	 * @return If the bind was successful, or not.
	 */
	bool(* bind)(VAO*);

	/**
	 * Unbinds the current vao.
	 *
	 * @return If the unbind was successful, or not.
	 */
	bool(* unbind)();

	/**
	 * Attaches a VBO to the given VAO and returns if it was successful.
	 *
	 * @param vao The VAO to attach to.
	 * @param vbo The VBO to attach to the VAO.
	 * @param attribLocation The attribute location to bind to.
	 * @param dataType The type of data that's in the VBO.
	 * @return If the VBO was successfully attached.
	 */
	bool(* attachVBO)(VAO*, VBO*, GLuint, GLenum);

	/**
	 * Attaches a EAB to the given VAO and returns if it was successful.
	 *
	 * @param vao The VAO to attach to.
	 * @param vbo The EAB to attach to the VAO.
	 * @param attribLocation The attribute location to bind to.
	 * @param dataType The type of data that's in the VBO.
	 * @return If the VBO was successfully attached.
	 */
	bool(* attachEAB)(VAO*, EAB*, GLuint, GLenum);

	/**
	 * Sets the VAO render information.
	 *
	 * @param vao The VAO to modify.
	 * @param vertexCount the number of vertices to draw.
	 */
	void (*setRenderInfo)(VAO*, uint32_t);

	/**
	 * Renders the given vao.
	 * @param vao
	 * @return Whether or not it was rendered.
	 */
	bool (* draw)(VAO* vao);

	/**
	 * Frees the VAO and all attached VBOs from the GPU and Heap.
	 * @param vao The VAO to clear.
	 */
	void(* delete)(VAO* vao);
};

typedef struct VAOManager_s VAOManager;

extern const VAOManager manVAO;

#endif /* SRC_GL_VAO_H_ */
