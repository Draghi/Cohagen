#include "TextureUtil.h"

#include "lib/ogl.h"
#include "gl/Textures.h"
#include "util/Bitmap.h"
#include "util/FileUtil.h"

#include <stdlib.h>

Texture* createTextureFromFile(char* filename, const GLint magFilter, const GLint minFilter) {
	Texture* tex = NULL;


	FileData f;
	if (fileUtil.loadFile(filename, &f) == FILE_SUCCEEDED) { //Load bitmap data

		Bitmap* bmp = manBitmap.new(f.data, f.size); //Parse data into a usable image.

		tex = manTex.new();
		manTex.setData(tex, 0, (uint8_t*)bmp->pixels, GL_RGBA8, GL_RGBA, bmp->width, bmp->height, magFilter, minFilter);
		manBitmap.delete(bmp);
	} else {
		printf("Failed to load texture: %s", filename);
	}

	if (f.data!=NULL)
		free(f.data);

	return tex;
}

const TextureUtil textureUtil = {createTextureFromFile};

