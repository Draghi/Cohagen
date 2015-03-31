#ifndef COH_VAO_H
#define COH_VAO_H

#include <stdlib.h>
#include <stdbool.h>

#include "../gl/VBO.h"

#include "../lib/ogl.h"
#include "../util/DynamicArray.h"


struct VAO_s {
	GLuint id;
	int32_t numVbo;
	int32_t primaryVBO;
	DynamicArray *vbos;
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
	 * Unbinds the vao.
	 *
	 * @param self The vao to bind.
	 * @return If the unbind was successful, or not. Normally, false means it wasn't bound in the first place.
	 */
	bool(* unbind)(VAO*);

	/**
	 * Attaches a VBO to the given vao and returns the index of the VBO in the VAO.
	 *
	 * @param vao The VAO to attach to.
	 * @param vbo The VBO to attach to the VAO.
	 * @param dataType The type of data that's in the VBO.
	 * @param countPerVertex The number of elements in the VBO per vertex.
	 * @return The index of the VBO in the VAO
	 */
	int32_t(* attachVBO)(VAO*, VBO*, GLenum);

	/**
	 * Sets the primary vbo that the vao uses for render information.
	 * AKA the number of vertices to draw.
	 * @param vao The vao to modify.
	 * @param id The id of the vbo to use.
	 */
	void (* setPrimaryVBO)(VAO*, int32_t);

	/**
	 * Returns the VBO at the given ID from the VAO.
	 * @param vao The vao to fetch the vbo from.
	 * @param id The id of the VBO to return.
	 * @return The VBO at the given ID.
	 */
	VBO* (* getVBO)(VAO*, int32_t);

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
	void(* delete)();
};

typedef struct VAOManager_s VAOManager;

extern const VAOManager manVAO;

#endif /* SRC_GL_VAO_H_ */
