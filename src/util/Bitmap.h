#ifndef COH_BMPPIXELS_H
#define COH_BMPPIXELS_H

#include <stdint.h>

typedef union Pixel_u {
	uint32_t i;
	struct {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};
} Pixel;

/**
 * A bitmap structure that contains the image data and height/width.
 */
typedef struct Bitmap_s {
	uint16_t height;
	uint16_t width;
	Pixel **pixels;
} Bitmap;

/**
 * Bitmap creation manager.
 * Provides methods to create/parse information from a byte array into a useable bitmap.
 */
typedef struct BitmapManager_s {

	/**
	 * Creates a new bitmap from the given data.
	 * @param data The data that represents a bitmap
	 * @param size The size of the data
	 * @return A newly constructed bitmap structure.
	 */
	Bitmap* (* new)(uint8_t*, uint64_t);

	/**
	 * Frees the memory a bitmap occupies.
	 * @param bitmap A pointer to the bitmap to free.
	 */
	void (* delete)(Bitmap*);
} BitmapManager;

/**
 * The bitmap manager instance.
 */
extern const BitmapManager bitmapManager;

#endif
