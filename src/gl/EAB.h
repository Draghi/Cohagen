#ifndef COH_EAB_H
#define COH_EAB_H

#include <stdbool.h>

#include "lib/ogl.h"

/**
 * A fake object struct that represents an OpenGL vertex buffer object.
 */
struct EAB_s {
	/**
	 * The OpenGL id of the EAB.
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
typedef struct EAB_s EAB;

/**
 * A fake object struct that provides various helper methods for the EAB object.
 */
struct EABManager_s {
	/**
	 * Creates a new EAB.
	 * @return A pointer to the new EAB.
	 */
	EAB*(* new)();

	/**
	 * Binds the EAB to the array buffer.
	 *
	 * @param EAB The EAB to bind
	 * @return If the bind was successful, or not.
	 */
	bool(* bind)(const EAB* const);

	/**
	 * Unbinds the EAB from the array buffer.
	 *
	 * @return If the unbind was successful, or not.
	 */
	bool(* unbind)();

	/**
	 * Creates or changes the data that the EAB references.
	 *
	 * @param EAB The EAB to alter.
	 * @param data A pointer to the data to bind.
	 * @param size The size in bytes of the data.
	 * @param usage The OpenGL usage hint (GL_STATIC_DRAW eg.)
	 * @return Whether the data was set or not.
	 */
	bool(* setData)(const EAB* const, void*, GLsizeiptr, GLenum);

	/**
	 * Substitutes part of the data that the EAB references with the given data/
	 *
	 * @param EAB The EAB to alter.
	 * @param data A pointer to the data to bind.
	 * @param size The size in bytes to add from the data.
	 * @param offset The offset into the EAB's data to start writing at, in bytes.
	 * @return Whether the data was substituted or not.
	 */
	bool(* subData)(const EAB* const, void*, GLsizeiptr, GLintptr);


	/**
	 * Sets the number of vertices and the number of elements per vertex for the EAB.
	 *
	 * @param EAB The EAB to alter.
	 * @param vertCount The number of vertices the EAB represents.
	 * @param countPerVert The number of elements that represents a vertex.
 	 * @param stride Number of bytes from the start of one element to the start of the next.
 	 * @param pointer Offset to start reading from.
	 */
	void(* setRenderInfo)(EAB* const, GLint, GLint, GLint, GLvoid*);

	/**
	 * Frees the buffer from both the Computer's and the GPU's memory.
	 * @par
	 */
	void(* delete)(EAB* const);
};

/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct EABManager_s EABManager;

/**
 * Expose the EAB.c's EABManager object.
 */
extern const EABManager manEAB;

#endif /* SRC_GL_EAB_H_ */
