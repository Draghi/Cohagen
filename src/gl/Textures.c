#include"Textures.h"

#include<GL/gl.h>
#include<stdlib.h>
#include<stdbool.h>

#define MAX_TEXTURE_SLOTS 16

static Texture* slots[MAX_TEXTURE_SLOTS];

//////////////////////
// Hidden Functions //
//////////////////////
/**
 * Finds and returns the index of the first empty slot in the slots array.
 * If it cannot find an empty slot, it will return -1.
 * @return The index of the next empty texture slot, -1 if there is none.
 */
static int32_t findOpenSlot() {
	int i;
	for(i=0; i<MAX_TEXTURE_SLOTS; i++) {
		if (slots[i] == NULL)
			return i;
	}

	return -1;
}

///////////////////
// Texture Class //
///////////////////
/**
 * Attempts to bind the texture to the given texture unit slot.
 * @param tex The texture to bind too the texture unit.
 * @param slotID The slot to attempt to bind too, must be bellow MAX_TEXTURE_SLOTS.
 * @return True if the texture is bound, false if the texture fails to bind.
 */
static bool bind(Texture *tex, GLuint slotID) {
	if (slotID<MAX_TEXTURE_SLOTS) {
		tex->slotID = slotID;

		if (slots[slotID]!=NULL)
			slots[slotID]->slotID=-1;

		slots[slotID] = tex;

		glActiveTexture(GL_TEXTURE0+slotID);
		glBindTexture(GL_TEXTURE_2D, tex->id);

		return true;
	} else {
		return false;
	}
}

/**
 * Attempts to unbind the texture.
 * @param tex The texture to attempt to unbind.
 * @return Whether the texture was unbound. Note: If another texture bound itself to the same slot, this will fail to unbind.
 */
static bool unbind(Texture *tex) {
	if (slots[tex->slotID] == tex) {
		glActiveTexture(GL_TEXTURE0+tex->slotID);
		glBindTexture(GL_TEXTURE_2D, 0);

		tex->slotID = -1;
		slots[tex->slotID] = NULL;
		return true;
	} else {
		tex->slotID = -1;
		return true;
	}
}

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
static bool setTextureData(Texture *tex, GLubyte *data, GLint internalFormat, GLint format, uint32_t width, uint32_t height, GLint minFilter, GLint magFilter) {
	int32_t slot = findOpenSlot();

	if (slot>=0) {
		bind(tex, slot);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		tex->width = width;
		tex->height = height;

		unbind(tex);

		return true;
	} else {
		return false;
	}
}



///////////////////////////////
//   Texture Manager Class   //
/**@todo Add texture loading */
///////////////////////////////

/**
 * Creates a new Texture object that represents an OpenGL texture.
 * @return A working Texture object.
 */
static Texture* createTexture() {
	Texture* tex = malloc(sizeof(Texture));

	glGenTextures(1, &(tex->id));
	tex->width = 0;
	tex->height = 0;
	tex->slotID = -1;
	tex->bind = bind;
	tex->unbind = unbind;
	tex->setTextureData = setTextureData;

	return tex;
}

/**
 * Creates a blank RGBA texture (all bytes set to 0)
 * @param width The width of the new texture.
 * @param height The height of the new texture.
 * @param minFilter The OpenGL minification filter to use (GL_NEAREST eg.)
 * @param magFilter The OpenGL magnification filter to use (GL_LINEAR eg.)
 * @return A new texture object describing the created Blank OpenGL texture.
 */
static Texture* createBlankRGBATexture(uint32_t width, uint32_t height, GLint minFilter, GLint magFilter) {
	Texture* tex = createTexture();

	uint32_t size = width * height * sizeof(GLubyte) * 4;
	GLubyte data[size];

	for(int i = 0; i < size; i++) {
		data[i] = 0;
	}

	if (setTextureData(tex, data, GL_RGBA8, GL_RGBA, width, height, minFilter, magFilter)) {
		return tex;
	} else {
		free(tex);
		return NULL;
	}
}

/**
 * Creates a noisy RGBA texture (all bytes set to random values)
 * @param width The width of the new texture.
 * @param height The height of the new texture.
 * @param minFilter The OpenGL minification filter to use (GL_NEAREST eg.)
 * @param magFilter The OpenGL magnification filter to use (GL_LINEAR eg.)
 * @return A new texture object describing the created Blank OpenGL texture.
 */
static Texture* createNoiseRGBATexture(uint32_t width, uint32_t height, GLint minFilter, GLint magFilter) {
	Texture* tex = createTexture();

	uint32_t size = width * height * sizeof(GLubyte) * 4;
	GLubyte data[size];

	for(int i = 0; i < size; i++) {
		data[i] = rand() % 256;
	}

	if (setTextureData(tex, data, GL_RGBA8, GL_RGBA, width, height, minFilter, magFilter)) {
		return tex;
	} else {
		free(tex);
		return NULL;
	}
}

////////////////////////
// Singleton Instance //
////////////////////////
/**
 * The instance of the "fake object" singleton for the TextureManager
 * Each element corresponds to the strut defined in the header, in order.
 * Do not, I repeat DO NOT mess with this object, unless you are certain about what you're doing.
 */
const TextureManager textureManager = {createTexture, createBlankRGBATexture, createNoiseRGBATexture};

