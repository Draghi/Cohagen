#ifndef COH_SKYBOX_H
#define COH_SKYBOX_H

#include "gl/VAO.h"
#include "gl/VBO.h"
#include "gl/Textures.h"
#include "gl/Shader.h"
#include "util/TextureUtil.h"
#include "util/FileUtil.h"
#include "util/Bitmap.h"

typedef struct Skybox_s {
	VAO *vao;
	Texture *tex;
} Skybox;

typedef struct SkyboxManager_s {
	Skybox *(*new)(	const char *const front, const char *const back,
					const char *const top, const char *const bottom,
					const char *const left, const char *const right);

	void (*draw)(const Skybox *const skybox, const Shader *shader, const char *cubeMapShaderName);
} SkyboxManager;

extern const SkyboxManager manSkybox;

#endif
