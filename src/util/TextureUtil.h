
#ifndef COH_TEXTUREUTIL_H
#define COH_TEXTUREUTIL_H
	#include "lib/ogl.h"
	#include "gl/Textures.h"

	typedef struct TextureUtil_s {
		Texture*( *createTextureFromFile)(char*, const GLint, const GLint);
	} TextureUtil;

	extern const TextureUtil textureUtil;

#endif