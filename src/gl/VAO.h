#ifndef SRC_GL_VAO_H_
#define SRC_GL_VAO_H_

#include <stdbool.h>
#include "../graphics.h"
#include "../util/DynamicArray.h"

struct VAO_s {
	GLuint id;
	uint32_t numVbo;
	DynamicArray *vbos;
};

typedef struct VAO_s VAO;

struct VAOManager_s {
	VAO* (* newVAO)();
	bool(* bind)(VAO*);
	bool(* unbind)(VAO*);
	int32_t(* attachVBO)(VAO*, VBO*, GLenum, uint32_t);
	int32_t(* attachNewVBO)(VAO*, void*, GLsizeiptr, GLenum, GLenum, uint32_t);
	void(* delete)();
};

typedef struct VAOManager_s VAOManager;

extern const VAOManager vaoManager;

#endif /* SRC_GL_VAO_H_ */
