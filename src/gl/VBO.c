//#define GL_GLEXT_PROTOTYPES

#include "../graphics.h"
#include <stdlib.h>
#include <stdbool.h>

static const VBO* boundBuffer;

////////////////////////
// Internal Functions //
////////////////////////

/**
 * Prepares a given VBO for fixed function pipeline rendering
 *
 * @param vbo The vbo to prepare, if null it will disable the given clientState.
 * @param clientState The type of VBO
 * @return Whether the VBO was bound successfully.
 */
static bool prepare(const VBO* const vbo, GLenum clientState) {
	if (vbo==NULL) {
		glDisableClientState(clientState);
	} else {
		glEnableClientState(clientState);
		bool flag = glVBO.bind(vbo);

		if (!flag)
			return false;

		switch(clientState) {
			case GL_VERTEX_ARRAY:
				glVertexPointer(vbo->countPerVert, GL_FLOAT, vbo->stride, vbo->pointer);
				break;

			case GL_NORMAL_ARRAY:
				glNormalPointer(GL_FLOAT, vbo->stride, vbo->pointer);
				break;

			case GL_COLOR_ARRAY:
				glColorPointer(vbo->countPerVert, GL_FLOAT, vbo->stride, vbo->pointer);
				break;

			case GL_TEXTURE_COORD_ARRAY:
				glTexCoordPointer(vbo->countPerVert, GL_FLOAT, vbo->stride, vbo->pointer);
				break;
		}

		glVBO.unbind(vbo);
	}
	return true;
}

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
 * @param vbo The VBO to unbind.
 * @return If the unbind was successful, or not. Normally, false means it wasn't bound.
 */
static bool unbind(const VBO* const vbo) {
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

		unbind(vbo);
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
 */
static void setRenderInfo(VBO* const vbo, GLint vertCount, GLint countPerVert, GLint stride, GLvoid *pointer) {
	vbo->vertCount=vertCount;
	vbo->countPerVert=countPerVert;
	vbo->stride = stride;
	vbo->pointer = pointer;
}

/**
 * Renders the given set of VBOs using the fixed function pipeline.
 * Any VBO can be nulled and excluded, though it wouldn't make much sense to exclude the verticies.
 *
 * @param verts The vertices to render.
 * @param norms The normals of each vertex.
 * @param colours The colour of each vertex.
 * @param texCoords The texture coords of each vertex.
 *
 * @return Whether any of the bindings failed and caused the object not to be rendered.
 */
static bool draw(const VBO* const verts, const VBO* const norms, const VBO* const colours, const VBO* const texCoords) {
	bool flag = false;

	if (verts!=NULL)
		flag |= !prepare(verts, GL_VERTEX_ARRAY);
	else
		prepare(NULL, GL_VERTEX_ARRAY);

	if (norms!=NULL)
		flag |= !prepare(norms, GL_NORMAL_ARRAY);
	else
		prepare(NULL, GL_NORMAL_ARRAY);

	if (colours!=NULL)
		flag |= !prepare(colours, GL_COLOR_ARRAY);
	else
		prepare(NULL, GL_COLOR_ARRAY);

	if (texCoords!=NULL)
		flag |= !prepare(texCoords, GL_TEXTURE_COORD_ARRAY);
	else
		prepare(NULL, GL_TEXTURE_COORD_ARRAY);

	if (!flag) {
		glDrawArrays(GL_TRIANGLES, 0, verts->vertCount);
		return true;
	} else {
		return false;
	}
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
const VBOManager glVBO = {new, bind, unbind, setData, subData, setRenderInfo, draw, delete};
