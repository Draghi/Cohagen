
#ifndef COH_FBO_H
#define COH_FBO_H

#include"RBO.h"
#include"../lib/ogl.h"
#include"../gl/Textures.h"
#include"../gl/FBO.h"
#include"../util/DynamicArray.h"
#include"../util/DynamicIntArray.h"

struct FBO_s {
	GLuint id;
	GLuint width;
	GLuint height;

	DynamicArray* textures;
	DynamicArray* texRelations;

	DynamicArray* renderbuffers;
};

typedef struct FBO_s FBO;

struct FBOManager_s {
	FBO*(* new)();
	bool(* bind)(FBO*);
	bool(* unbind)(FBO*);
	bool(* attachTexture)(FBO*, Texture*, GLenum);
	bool(* attachRBO)(FBO*, RBO*, GLenum);
	void(* delete)(FBO*);
};

typedef struct FBOManager_s FBOManager;

extern const FBOManager manFBO;


#endif /* GL_FBO_H_ */
