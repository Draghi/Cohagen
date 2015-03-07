//#define GL_GLEXT_PROTOTYPES

//#include <GL/glew.h>
#include "VBO.h"
#include <stdlib.h>

static VBO* createVBO(){
	VBO* vbo = malloc(sizeof(VBO));

	//glGenBuffers(1, (&vbo->id));

	return vbo;
}

const VBOManager vboManager = {createVBO};
