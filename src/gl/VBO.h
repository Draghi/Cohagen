#ifndef SRC_GL_VBO_H_
#define SRC_GL_VBO_H_

#include"../graphics.h"
#include <stdbool.h>

struct VBO_s {
	GLenum id;
	bool(* bind)(struct VBO_s*);
	bool(* unbind)(struct VBO_s*);
	bool(* setData)(struct VBO_s*, void*, GLsizeiptr, GLenum);
	bool(* subData)(struct VBO_s*, void*, GLsizeiptr, GLintptr);
};

typedef struct VBO_s VBO;

struct VBOManager_s {
	VBO*(* createVBO)();
};

typedef struct VBOManager_s VBOManager;

extern const VBOManager vboManager;

#endif /* SRC_GL_VBO_H_ */
