#ifndef SRC_GL_VBO_H_
#define SRC_GL_VBO_H_

#include"../graphics.h"
#include <stdbool.h>

/**
 * A fake object struct that represents an OpenGL vertex buffer object.
 */
struct VBO_s {
	/**
	 * The OpenGL id of the VBO.
	 */
	GLenum id;

	/**
	 * Binds the vbo to the array buffer.
	 *
	 * @param self The pointer to the instance of the vbo.
	 * @return If the bind was successful, or not.
	 */
	bool(* bind)(struct VBO_s*);

	/**
	 * Unbinds the vbo from the array buffer.
	 *
	 * @param self The pointer to the instance of the vbo.
	 * @return If the unbind was successful, or not. Normally, false means it wasn't bound.
	 */
	bool(* unbind)(struct VBO_s*);

	/**
	 * Creates or changes the data that the VBO references.
	 *
	 * @param self The pointer to the instance of the vbo.
	 * @param data A pointer to the data to bind.
	 * @param size The size in bytes of the data.
	 * @param usage The OpenGL usage hint (GL_STATIC_DRAW eg.)
	 * @return Whether the data was set or not.
	 */
	bool(* setData)(struct VBO_s*, void*, GLsizeiptr, GLenum);

	/**
	 * Substitutes part of the data that the VBO references with the given data/
	 *
	 * @param self The pointer to the instance of the vbo.
	 * @param data A pointer to the data to bind.
	 * @param size The size in bytes to add from the data.
	 * @param offset The offset into the VBO's data to start writing at, in bytes.
	 * @return Whether the data was substituted or not.
	 */
	bool(* subData)(struct VBO_s*, void*, GLsizeiptr, GLintptr);

	/**
	 * Frees the buffer from both the Computer's and the GPU's memory.
	 * @par
	 */
	void(* delete)(struct VBO_s* self);
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
	VBO*(* createVBO)();
};

/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct VBOManager_s VBOManager;

/**
 * Expose the Vbo.c's VBOManager object.
 */
extern const VBOManager vboManager;

#endif /* SRC_GL_VBO_H_ */
