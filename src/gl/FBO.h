#ifndef FBO_H
#define FBO_H

#include"lib/ogl.h"
#include"gl/Textures.h"
#include"gl/FBO.h"
#include"gl/RBO.h"
#include"util/DynamicArray.h"

/**
 *	Framebuffer object, represents an OpenGL
 * 	Framebuffer object.
 */
struct FBO_s {
	GLuint id;
	GLuint width;
	GLuint height;

	DynamicArray* textures;
	DynamicArray* texRelations;

	DynamicArray* renderbuffers;
};

typedef struct FBO_s FBO;

/**
 *	Class used to manage Framebuffer objects.
 */
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

#endif /* FBO_H */
