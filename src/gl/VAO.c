#include "../graphics.h"
#include "../util/DynamicArray.h"
#include <stdbool.h>

static VAO* boundBuffer = NULL;

/////////////////
// VAO Manager //
/////////////////
static VAO* newVAO() {
	VAO* vao = malloc(sizeof(VAO));

	glGenVertexArrays(1, &(vao->id));
	vao->numVbo=0;
	newDynamicArray((vao->vbos), 0, sizeof(void*));

	return vao;
}
/**
 * Binds the vao.
 *
 * @param self The pointer to the instance of the vao.
 * @return If the bind was successful, or not.
 */
static bool bind(VAO* vao) {
	if (boundBuffer!=vao) {
		boundBuffer = vao;
		glBindVertexArray(vao->id);

		return true;
	} else {
		return false;
	}
}

/**
 * Unbinds the vao.
 *
 * @param self The pointer to the instance of the cao.
 * @return If the unbind was successful, or not. Normally, false means it wasn't bound in the first place.
 */
static bool unbind(VAO* vao) {
	if (boundBuffer==vao) {
		boundBuffer = NULL;
		glBindVertexArray(0);

		return true;
	} else {
		return false;
	}
}

/**
 * Attaches a VBO to the given vao and returns the index of the VBO in the VAO.
 *
 * @param vao The VAO to attach to.
 * @param vbo The VBO to attach to the VAO.
 * @param dataType The type of data that's in the VBO.
 * @param countPerVertex The number of elements in the VBO per vertex.
 * @return The index of the VBO in the VAO
 */
static int32_t attachVBO(VAO* vao, VBO* vbo, GLenum dataType, uint32_t countPerVertex) {
	if (vbo!=NULL) {
		if ((bind(vao)) && (vboManager.bind(vbo))) {
			glVertexAttribPointer(vbo->id, countPerVertex, dataType, false, 0, 0);

			vao->vbos->append(vao->vbos, vbo);
	        (vao->numVbo)++;

			return vao->numVbo-1;
		}

		unbind(vao);
		vboManager.unbind(vbo);
	}

	return -1;
}

/**
 * Creates and attaches a VBO to the given vao and returns the index of the VBO in the VAO.
 *
 * @param vao The VAO to attach to.
 * @param vbo The VBO to attach to the VAO.
 * @param data The data to fill the VBO with
 * @param size The number of bytes of data to fill the VBO with.
 * @param usageType The OpenGL usage type hint.
 * @param dataType The type of data that's in the VBO.
 * @param countPerVertex The number of elements in the VBO per vertex.
 * @return The index of the VBO in the VAO
 */
static int32_t attachNewVBO(VAO* vao, void* data, GLsizeiptr size, GLenum usageType,  GLenum dataType, uint32_t countPerVertex) {
	VBO* vbo = vboManager.createVBO();
	vboManager.setData(vbo, data, size, usageType);

	int32_t res = attachVBO(vao, vbo, dataType, countPerVertex);
	if (res<0)
		vboManager.delete(vbo);

	return res;
}

/**
 * Frees the VAO and all attached VBOs from the GPU and Heap.
 * @param vao The VAO to clear.
 */
static void delete(VAO* vao) {
	glDeleteVertexArrays(1, &(vao->id));

	for(int i = 0; i<vao->numVbo; i++) {
		vboManager.delete((VBO *)vao->vbos->get(vao->vbos, i));
	}

	deleteDynamicArray(vao->vbos);
	free(vao->vbos);
}

const VAOManager vaoManager = {newVAO, bind, unbind, attachVBO, attachNewVBO, delete};
