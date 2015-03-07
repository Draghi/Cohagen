#ifndef SRC_GL_VBO_H_
#define SRC_GL_VBO_H_

#include"../graphics.h"

struct VBO_s {
	GLuint id;
};

typedef struct VBO_s VBO;

struct VBOManager_s {
	VBO*(* createVBO)();
};

typedef struct VBOManager_s VBOManager;

extern const VBOManager vboManager;

#endif /* SRC_GL_VBO_H_ */
