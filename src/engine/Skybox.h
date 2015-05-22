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
	/**
	 *	Creates a Skybox object that uses the given images and returns
	 * 	a pointer to that Skybox object.
	 *
	 * 	@param 	front 	const pointer to const char, path to front image.
	 * 	@param 	back 	const pointer to const char, path to back image.
	 * 	@param 	top 	const pointer to const char, path to top image.
	 * 	@param 	bottom 	const pointer to const char, path to bottom image.
	 * 	@param 	left 	const pointer to const char, path to left image.
	 * 	@param 	right 	const pointer to const char, path to right image.
	 * 	@return 		pointer to Skybox object.
	 */
	Skybox *(*new)(	const char *const front, const char *const back,
					const char *const top, const char *const bottom,
					const char *const left, const char *const right);

	/**
	 *	Creates a Skybox from the images that use the base name in the given path.
	 * 	If the base name is "skybox", the images used will be "skybox_front.bmp",
	 * 	"skybox_back.bmp", "skybox_left.bmp", "skybox_right.bmp", "skybox_top.bmp", "skybox_bottom.bmp".
	 *
	 * 	@param 		path 		const pointer to const char, path to folder containing skybox images.
	 * 	@param 		baseName	const pointer to const char, base name for skybox image.
	 * 	@returns 				pointer to Skybox object.
	 */
	Skybox *(*newFromGroup)(const char *const path, const char *const baseName);

	/**
	 *	Draws skybox using the given shader.
	 *
	 * 	@param 	skybox 				const pointer to const Skybox, skybox to draw.
	 * 	@param 	shader 				pointer to const shader, shader to use.
	 * 	@param 	cubeMapShaderName 	const pointer to char, name of cubeMap sampler in shader.
	 */
	void (*draw)(const Skybox *const skybox, const Shader *const shader, const char *const cubeMapShaderName);
} SkyboxManager;

extern const SkyboxManager manSkybox;

#endif
