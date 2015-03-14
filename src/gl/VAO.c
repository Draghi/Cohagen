#include "../graphics.h"
#include "../util/DynamicArray.h"
#include <stdbool.h>

static VAO* boundBuffer = NULL;

/////////////////
// VAO Manager //
/////////////////
/**
 * Creates a new VAO.
 * @return A new VAO
 */
static VAO* newVAO() {
	VAO* vao = malloc(sizeof(VAO));

	glGenVertexArrays(1, &(vao->id));
	vao->numVbo=0;
	vao->primaryVBO = -1;
	newDynamicArray((vao->vbos), 0, sizeof(void*));

	return vao;
}

/**
 * Binds the vao.
 *
 * @param vao The vao to bind.
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
 * @param self The vao to bind.
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
static int32_t attachVBO(VAO* vao, VBO* vbo, GLenum dataType) {
	if (vbo!=NULL) {
		if ((bind(vao)) && (vboManager.bind(vbo))) {
			glVertexAttribPointer(vbo->id, vbo->countPerVert, dataType, GL_FALSE, vbo->stride, vbo->pointer);

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
 * Sets the primary vbo that the vao uses for render information.
 * AKA the number of vertices to draw.
 * @param vao The vao to modify.
 * @param id The id of the vbo to use.
 */
static void setPrimaryVBO(VAO* vao, int32_t id) {
	vao->primaryVBO = id;
}

/**
 * Returns the VBO at the given ID from the VAO.
 * @param vao The vao to fetch the vbo from.
 * @param id The id of the VBO to return.
 * @return The VBO at the given ID.
 */
static VBO* getVBO(VAO* vao, int32_t id) {
	return (VBO*) vao->vbos->get(vao->vbos, id);
}

/**
 * Renders the given vao.
 * @param vao
 * @return Whether or not it was rendered.
 */
static bool draw(VAO* vao) {
	if (bind(vao)) {
		if (vao->primaryVBO>=0) {
			VBO* renderInfo = (VBO*) vao->vbos->get(vao->vbos, vao->primaryVBO);
			glDrawArrays(GL_TRIANGLES, 0, renderInfo->vertCount);
			return true;
		}
	}
	unbind(vao);
	return false;
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

const VAOManager vaoManager = {newVAO, bind, unbind, attachVBO, setPrimaryVBO, getVBO, draw, delete};
