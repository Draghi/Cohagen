#include "VAO.h"

#include <stdbool.h>
#include <stdio.h>

#include "lib/ogl.h"
#include "util/DynamicArray.h"

static VAO* new() {
	VAO* vao = malloc(sizeof(VAO));

	glGenVertexArrays(1, &(vao->id));

	return vao;
}

static bool bind(VAO* vao) {
	glBindVertexArray(vao->id);

	return true;
}

static bool unbind() {
	glBindVertexArray(0);
	return true;
}

static bool attachVBO(VAO* vao, VBO* vbo, GLuint attribLocation, GLenum dataType) {
	if (vbo!=NULL) {
		if ((bind(vao)) && (manVBO.bind(vbo))) {
		    glEnableVertexAttribArray(attribLocation);
		    glVertexAttribPointer(attribLocation, vbo->countPerVert, dataType, GL_FALSE, vbo->stride, vbo->pointer);

			return true;
		}

		unbind();
		manVBO.unbind();
	}

	return false;
}

static bool attachEAB(VAO* vao, EAB* eab, GLuint attribLocation, GLenum dataType) {
	if (eab!=NULL) {
		if ((bind(vao)) && (manEAB.bind(eab))) {
		    glEnableVertexAttribArray(attribLocation);
		    glVertexAttribPointer(attribLocation, eab->countPerVert, dataType, GL_FALSE, eab->stride, eab->pointer);

			return true;
		}

		unbind();
		manVBO.unbind();
	}

	return false;
}

static void setRenderInfo(VAO* vao, uint32_t vertCount) {
	vao->vertCount = vertCount;
}

static bool draw(VAO* vao) {
	if (bind(vao)) {
		glDrawArrays(GL_TRIANGLES, 0, vao->vertCount);
		unbind();
		return true;
	}
	return false;
}

static void delete(VAO* vao) {
	glDeleteVertexArrays(1, &(vao->id));
}

const VAOManager manVAO = {new, bind, unbind, attachVBO, attachEAB, setRenderInfo, draw, delete};
