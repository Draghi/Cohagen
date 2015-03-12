#ifndef COH_TEXTURES_H
#define COH_TEXTURES_H

#include<stdint.h>
#include<stdbool.h>

#include"../graphics.h"

/**
 * Fake texture object.
 * Describes an OpenGL texture and provides methods to bind/unbind and change data.
 */
struct Texture_s {
	uint32_t id;
	uint32_t width;
	uint32_t height;

	int8_t slotID;
};


/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct Texture_s Texture;

/**
 * Fake texture manager object.
 * Provides methods to create texture objects with various different initial textures.
 */
struct TextureManager_s {
	/**
	 * Creates a new Texture object that represents an OpenGL texture.
	 * @return A working Texture object.
	 */
	Texture*(* newTexture)();

	/**
	 * Creates a blank RGBA texture (all bytes set to 0)
	 * @param width The width of the new texture.
	 * @param height The height of the new texture.
	 * @param minFilter The OpenGL minification filter to use (GL_NEAREST eg.)
	 * @param magFilter The OpenGL magnification filter to use (GL_LINEAR eg.)
	 * @return A new texture object describing the created Blank OpenGL texture.
	 */
	Texture*(* newBlankRGBATexture)(const uint32_t, const uint32_t, const GLint, const GLint);

	/**
	 * Creates a noisy RGBA texture (all bytes set to random values)
	 * @param width The width of the new texture.
	 * @param height The height of the new texture.
	 * @param minFilter The OpenGL minification filter to use (GL_NEAREST eg.)
	 * @param magFilter The OpenGL magnification filter to use (GL_LINEAR eg.)
	 * @return A new texture object describing the created Blank OpenGL texture.
	 */
	Texture*(* newNoiseRGBATexture)(const uint32_t, uint32_t, const GLint, const GLint);

	/**
	 * Attempts to bind the texture to the given texture unit slot.
	 * @param tex The texture to bind too the texture unit.
	 * @param slotID The slot to attempt to bind too, must be bellow MAX_TEXTURE_SLOTS.
	 * @return True if the texture is bound, false if the texture fails to bind.
	 */
	bool(* bind)(struct Texture_s *const, const GLuint);

	/**
	 * Attempts to unbind the texture.
	 * @param tex The texture to attempt to unbind.
	 * @return Whether the texture was unbound. Note: If another texture bound itself to the same slot, this will fail to unbind.
	 */
	bool(* unbind)(struct Texture_s *const);

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
	bool(* setData)(Texture* const, const GLubyte*const , const GLint, const GLint, const uint32_t, const uint32_t, const GLint, const GLint);

	void(* delete)(Texture* const);
};


/**
 * Typedef so we don't have too use "struct" everytime.
 */
typedef struct TextureManager_s TextureManager;

/**
 * Expose the Textures.c's textureManager object.
 */
extern const TextureManager textureManager;

#endif /* SRC_GL_TEXTURES_H_ */
