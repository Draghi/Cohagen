#ifndef COH_TEXTURES_H
#define COH_TEXTURES_H

#include <stdint.h>
#include <stdbool.h>

#include "lib/ogl.h"

#define TEX_GEN_BLACK 0x0000
#define TEX_GEN_WHITE 0x0001
#define TEX_GEN_NOISE 0x0002

/**
 * Struct to store information about an OpenGL texture.
 */
struct Texture_s {
	uint32_t id;
	uint32_t width;
	uint32_t height;

	int8_t slotID;
};

/**
 * Struct to store information about an OpenGL texture.
 */
typedef struct Texture_s Texture;

/**
 * Fake object to manage the creation, removal and use of textures.
 */
struct TextureManager_s {

	/**
	 * Creates a new Texture struct on the heap and links it to a new OpenGL texture.
	 * @return A new Texture struct linked to an OpenGL texture.
	 */
	Texture*(* new)();

	/**
	 * Attempts to bind the texture.
	 * @param tex 		The texture to attempt to bind.
	 * @param target 	GLenum, Texture binding target (eg. GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP)
	 * @param slotID 	The slot to attempt to bind too, must be bellow MAX_TEXTURE_SLOTS.
	 * @return 			True if the texture is bound, false if the texture fails to bind.
	 */
	bool(* bind)(struct Texture_s *const, GLenum target, const GLuint);

	/**
	 * Attempts to unbind the texture.
	 * @param 	tex 	The texture to attempt to unbind.
	 * @param 	target 	GLenum, target to attempt to unbind.
	 * @return Whether the texture was unbound. Note: If another texture bound itself to the same slot, this will fail to unbind.
	 */
	bool(* unbind)(struct Texture_s *const, GLenum target);

	/**
	 * Fills the given texture with data generated by the given params.
	 *
	 * @param texture The texture to alter.
	 * @param genType The type of generator to use (TEX_GEN_BLACK eg.)
	 * @param width The width of the texture.
	 * @param height The height of the texture.
	 * @param internalFormat The internal OpenGL format (GL_RGBA8 eg.)
	 * @param format The OpenGL format (GL_RGBA eg.)
	 * @param minFilter The OpenGL minification filter to use (GL_NEAREST eg.)
	 * @param magFilter The OpenGL magnification filter to use (GL_LINEAR eg.)
	 * @return A new texture object describing the created Blank OpenGL texture.
	 */
	bool(* genData)(Texture *const, const int slot, const uint32_t, const uint32_t, const uint32_t, const GLenum, const GLint, const GLint, const GLint);

	/**
	 * Attempts to set the given textures data
	 * @param tex The texture to change the data of.
	 * @param data The data to change to.
	 * @param internalFormat The internal OpenGL format (GL_RGBA8 eg.)
	 * @param format The OpenGL format (GL_RGBA eg.)
	 * @param width The width of the texture.
	 * @param height The height of the texture.
	 * @param minFilter The OpenGL minification filter to use (GL_NEAREST eg.)
	 * @param magFilter The OpenGL magnification filter to use (GL_LINEAR eg.)
	 * @return Whether or not the data was changed. Usually only false when all texture slots are occupied.
	 */
	bool(* setData)(Texture* const, const int slot, const GLubyte*const , const GLint, const GLint, const uint32_t, const uint32_t, const GLint, const GLint);

	/**
	 *	Specify a two-dimensional texture image and bind to specified texture.
	 *
	 * 	@param 	tex 			const pointer to Texture, The texture object to bind to.
	 * 	@param 	slot 			int, texture slot to bind to.
	 * 	@param 	data 			const pointer to const GLubyte, image data.
	 * 	@param 	target 			GLenum, target texture to bind to.
	 * 	@param 	mipmapLevel		GLint, level of detail number, 0 is base image level.
	 * 	@param 	internalFormat 	GLint, number of colour components in the texture (eg. GL_SRGB8).
	 *  @param 	format 			GLenum, The OpenGL format (eg. GL_RGBA).
	 * 	@param 	type 			GLenum, data type of the pixel data.
	 * 	@param 	width 			unsigned int, width of the texture.
	 * 	@param 	height 			unsigned int, height of the texture.
	 */
	void(* imageToTarget)(Texture *const tex, int slot, const GLubyte* const data, GLenum target, GLint mipmapLevel, GLint internalFormat, GLenum format, GLenum type, const uint32_t width, const uint32_t height);

	/**
	 *	Set texture parameters.
	 *
	 *	@param 	tex 	const pointer to Texture to set parameters of.
	 * 	@param 	slot 	int, texture slot to bind to.
	 * 	@param 	target 	GLenum, target texture of active texture unit.
	 * 	@param 	pname 	GLenum, name of parameter.
	 * 	@param 	value 	GLint, value of parameter.
	 */
	void(* formatTexture)(Texture *const tex, int slot, GLenum target, GLenum pname, GLint value);

	/**
	 * Frees the given texture from both the GPU and System memory.
	 * @param The texture to free.
	 */
	void(* delete)(Texture* const);
};

/**
 * Fake object to manage the creation, removal and use of textures.
 */
typedef struct TextureManager_s TextureManager;

/**
 * Fake object to manage the creation, removal and use of textures.
 */
extern const TextureManager manTex;

#endif /* SRC_GL_TEXTURES_H_ */
