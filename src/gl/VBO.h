#ifndef COH_VBO_H
#define COH_VBO_H

#include"../lib/ogl.h"

#include <stdbool.h>

/**
 * A fake object struct that represents an OpenGL vertex buffer object.
 */
struct VBO_s {
	/**
	 * The OpenGL id of the VBO.
	 */
	GLenum id;
	GLint vertCount;
	GLint countPerVert;
	GLint stride;
	GLvoid *pointer;
};

/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct VBO_s VBO;

/**
 * A fake object struct that provides various helper methods for the VBO object.
 */
struct VBOManager_s {
	/**
	 * Creates a new VBO.
	 * @return A pointer to the new VBO.
	 */
	VBO*(* new)();

	/**
	 * Binds the vbo to the array buffer.
	 *
	 * @param vbo The VBO to bind
	 * @return If the bind was successful, or not.
	 */
	bool(* bind)(const VBO* const);

	/**
	 * Unbinds the vbo from the array buffer.
	 *
	 * @param vbo The VBO to unbind
	 * @return If the unbind was successful, or not. Normally, false means it wasn't bound.
	 */
	bool(* unbind)(const VBO* const);

	/**
	 * Creates or changes the data that the VBO references.
	 *
	 * @param vbo The VBO to alter.
	 * @param data A pointer to the data to bind.
	 * @param size The size in bytes of the data.
	 * @param usage The OpenGL usage hint (GL_STATIC_DRAW eg.)
	 * @return Whether the data was set or not.
	 */
	bool(* setData)(const VBO* const, void*, GLsizeiptr, GLenum);

	/**
	 * Substitutes part of the data that the VBO references with the given data/
	 *
	 * @param vbo The VBO to alter.
	 * @param data A pointer to the data to bind.
	 * @param size The size in bytes to add from the data.
	 * @param offset The offset into the VBO's data to start writing at, in bytes.
	 * @return Whether the data was substituted or not.
	 */
	bool(* subData)(const VBO* const, void*, GLsizeiptr, GLintptr);


	/**
	 * Sets the number of vertices and the number of elements per vertex for the VBO.
	 *
	 * @param vbo The VBO to alter.
	 * @param vertCount The number of vertices the VBO represents.
	 * @param countPerVert The number of elements that represents a vertex.
	 */
	void(* setRenderInfo)(VBO* const, GLint, GLint, GLint, GLvoid*);

	/**
	 * Frees the buffer from both the Computer's and the GPU's memory.
	 * @par
	 */
	void(* delete)(VBO* const);
};

/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct VBOManager_s VBOManager;

/**
 * Expose the Vbo.c's VBOManager object.
 */
extern const VBOManager manVBO;

#endif /* SRC_GL_VBO_H_ */
