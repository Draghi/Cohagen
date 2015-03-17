#include"../graphics.h"

#include<stdlib.h>
#include<stdbool.h>
#include<math.h>


static Texture* slots[TEX_MAX_SLOTS];

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
	for(i=0; i<TEX_MAX_SLOTS; i++) {
		if (slots[i] == NULL)
			return i;
	}

	return -1;
}

///////////////////////////////
//   Texture Manager Class   //
/**@todo Add texture loading */
///////////////////////////////


/**
 * Creates a new Texture stuct on the heap and links it to a new OpenGL texture.
 * @return A new Texture stuct linked to an OpenGL texture.
 */
static Texture* new() {
	Texture* tex = malloc(sizeof(Texture));

	glGenTextures(1, &(tex->id));
	tex->width = 0;
	tex->height = 0;
	tex->slotID = -1;

	return tex;
}

/**
 * Attempts to bind the texture to the given texture unit slot.
 * @param tex The texture to bind too the texture unit.
 * @param slotID The slot to attempt to bind too, must be bellow MAX_TEXTURE_SLOTS.
 * @return True if the texture is bound, false if the texture fails to bind.
 */
static bool bind(Texture * const tex, const GLuint slotID) {
	if (slotID<TEX_MAX_SLOTS) {
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
static bool unbind(Texture * const tex) {
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
static bool setData(Texture *const tex, const GLubyte *const data, const GLint internalFormat, const GLint format, const uint32_t width, const uint32_t height, const GLint minFilter, const GLint magFilter) {
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
static bool genData(Texture *const tex, const uint32_t genType, const uint32_t width, const uint32_t height, const GLenum internalFormat, const GLint format, const GLint minFilter, const GLint magFilter) {
	uint32_t size = width * height * manOGLUtil.getPixelSize(internalFormat);
	GLubyte data[size];

	for(uint32_t i = 0; i < size; i++) {
		switch(genType) {
			case(TEX_GEN_BLACK):
				data[i] = 0;
				break;
			case(TEX_GEN_WHITE):
				data[i] = 255;
				break;
			case(TEX_GEN_NOISE):
				data[i] = rand() % 256;
				break;
		}
	}

	return setData(tex, data, internalFormat, format, width, height, minFilter, magFilter);
}

/**
 * Frees the given texture from both the GPU and System memory.
 * @param The texture to free.
 */
static void delete(Texture* const tex) {
	glDeleteTextures(1, &(tex->id));
	free(tex);
}

////////////////////////
// Singleton Instance //
////////////////////////
/**
 * The instance of the "fake object" singleton for the TextureManager
 * Each element corresponds to the strut defined in the header, in order.
 * Do not, I repeat DO NOT mess with this object, unless you are certain about what you're doing.
 */
const TextureManager manTex = {new, bind, unbind, genData, setData, delete};

