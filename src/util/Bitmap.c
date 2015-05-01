#include "Bitmap.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <util/MemUtil.h>

typedef struct {
	uint32_t mask;
	uint32_t shiftin;
	float maxValue;
} BmpMask;

typedef enum {
	BMP_DIB_V0 = 12,
	BMP_DIB_V1 = 40,
	BMP_DIB_V2 = 52,
	BMP_DIB_V3 = 56,
	BMP_DIB_V4 = 108,
	BMP_DIB_V5 = 124
} Bmp_Dib_Versions;

typedef struct {
	int32_t  imageWidth;
	int32_t  imageHeight;

	int16_t  bitsPerPixel;

	uint32_t compressionMode;
	uint32_t paletteSize;

	uint32_t maskRed;
	uint32_t maskGreen;
	uint32_t maskBlue;
	uint32_t maskAlpha;
} BmpDib;

typedef struct {
	uint16_t type;
	uint32_t fileSize;
	uint16_t meta1;
	uint16_t meta2;
	uint32_t pixelOffset;
} BmpFileHeader;


typedef struct {
	//Fields from the BitmapCoreHeader format.
	int32_t   imageWidth;
	int32_t   imageHeight;
	int16_t   bitPlanes;
	int16_t   bitsPerPixel;

	//Fields from the BitmapInfoHeader format.
	uint32_t compressionMode;
	uint32_t imageSize;
	uint32_t resHorz;
	uint32_t resVert;
	uint32_t paletteSize;
	uint32_t paletteNumImportant;

	//Fields from the BitmapV2InfoHeader format.
	uint32_t maskRed;
	uint32_t maskGreen;
	uint32_t maskBlue;

	//Fields from the BitmapV3InfoHeader format.
	uint32_t maskAlpha;
} BmpDibV3;

static void loadFileHeaderData(BmpFileHeader* dest, uint8_t* data) {
	uint32_t off = 0;

	off = memUtil.fillBytes(&dest->type,        data, off, sizeof(uint16_t));
	off = memUtil.fillBytes(&dest->fileSize,    data, off, sizeof(uint32_t));
	off = memUtil.fillBytes(&dest->meta1,       data, off, sizeof(uint16_t));
	off = memUtil.fillBytes(&dest->meta2,       data, off, sizeof(uint16_t));
	off = memUtil.fillBytes(&dest->pixelOffset, data, off, sizeof(uint32_t));
}

static BmpFileHeader* newFileHeader(uint8_t* data, uint64_t size) {
	if (size>=14) {
		BmpFileHeader* bfh = malloc(sizeof(BmpFileHeader));
		loadFileHeaderData(bfh, data);
		return bfh;
	}

	return NULL;
}

static uint64_t loadV3Header(BmpDib* dib, uint8_t* data) {
	BmpDibV3 dest; //= (BmpDibV3*) &(dib->header);
	uint64_t off = 0;

	off = memUtil.fillBytes(&dest.imageWidth, data, off, sizeof(int32_t));
	off = memUtil.fillBytes(&dest.imageHeight, data, off, sizeof(int32_t));
	off = memUtil.fillBytes(&dest.bitPlanes, data, off, sizeof(uint16_t));
	off = memUtil.fillBytes(&dest.bitsPerPixel, data, off, sizeof(uint16_t));

	off = memUtil.fillBytes(&dest.compressionMode, data, off, sizeof(uint32_t));
	off = memUtil.fillBytes(&dest.imageSize, data, off, sizeof(uint32_t));
	off = memUtil.fillBytes(&dest.resHorz, data, off, sizeof(uint32_t));
	off = memUtil.fillBytes(&dest.resVert, data, off, sizeof(uint32_t));
	off = memUtil.fillBytes(&dest.paletteSize, data, off, sizeof(uint32_t));
	off = memUtil.fillBytes(&dest.paletteNumImportant, data, off, sizeof(uint32_t));

	off = memUtil.fillBytes(&dest.maskRed,   data, off, sizeof(uint32_t));
	off = memUtil.fillBytes(&dest.maskGreen, data, off, sizeof(uint32_t));
	off = memUtil.fillBytes(&dest.maskBlue,  data, off, sizeof(uint32_t));

	off = memUtil.fillBytes(&dest.maskAlpha,  data, off, sizeof(uint32_t));

	dib->imageWidth = dest.imageWidth;
	dib->imageHeight = dest.imageHeight;
	dib->bitsPerPixel = dest.bitsPerPixel;
	dib->compressionMode = dest.compressionMode;
	dib->maskRed   = dest.maskRed;
	dib->maskGreen = dest.maskGreen;
	dib->maskBlue  = dest.maskBlue;
	dib->maskAlpha = dest.maskAlpha;
	dib->paletteSize = dest.paletteSize;

	return off;
}

BmpDib* newBitmapDIB(uint8_t* data, uint64_t size) {
	BmpDib* res = malloc(sizeof(BmpDib));

	uint32_t headerSize;
	memUtil.fillBytes(&headerSize, data, 0, sizeof(uint32_t));

	if (size>=headerSize) {
		switch(headerSize) {
			case BMP_DIB_V3:
			case BMP_DIB_V4:
			case BMP_DIB_V5:
				loadV3Header(res, data+sizeof(uint32_t));
				break;

			case BMP_DIB_V0:
			case BMP_DIB_V1:
			case BMP_DIB_V2:
				printf("Unsupported bitmap version!");
				exit(-1);
		}
		return res;
	}

	free(res);
	return NULL;
}

void calcMasks(uint32_t* tmp, BmpMask* masks, uint32_t channels) {
	uint32_t mask;
	int nbits, offset, bit;

	for (uint32_t i = 0; i < channels; i++) {
		mask = tmp[i];
		masks[i].mask = mask;
		nbits = 0;
		offset = -1;
		for (bit = 0; bit < 32; bit++) {
			if (mask & 1){
				nbits++;
				if (offset == -1)
					offset = bit;
			}
			mask = mask >> 1;
		}
		masks[i].shiftin = offset;
		masks[i].maxValue = (float)((1<<(nbits))-1);
	}
}

Bitmap* loadBmp(uint8_t* data, BmpDib* dib) {
	Bitmap* dest = malloc(sizeof(Bitmap));

	dest->height = abs(dib->imageHeight);
	dest->width = abs(dib->imageWidth);
	dest->pixels = malloc(sizeof(Pixel)*dest->height*dest->width);//(Pixel**) memUtil.malloc2D(sizeof(Pixel), dest->height, dest->width);

	uint32_t tmp[4] = {dib->maskRed, dib->maskGreen, dib->maskBlue, dib->maskAlpha};
	BmpMask masks[4];
	calcMasks(tmp, masks, 4);

	uint64_t offset = 0;
	uint32_t bytesPerPixel = dib->bitsPerPixel/8.0+0.5;
	uint32_t bytesRow = (dest->width *bytesPerPixel);
	uint32_t paddingBytes = (bytesRow % 4) ? 4 - (bytesRow % 4) : 0;

	uint32_t raw = 0;
	for(uint32_t i = 0; i<dest->height; i++) {
		for(uint32_t j = 0; j<dest->width; j++) {
			memUtil.fillBytes(&raw, data, offset, sizeof(uint32_t));

			dest->pixels[i+j*dest->width].r = ((raw & masks[0].mask) >> masks[0].shiftin) * 255.0 / masks[0].maxValue + 0.5;
			dest->pixels[i+j*dest->width].g = ((raw & masks[1].mask) >> masks[1].shiftin) * 255.0 / masks[1].maxValue + 0.5;
			dest->pixels[i+j*dest->width].b = ((raw & masks[2].mask) >> masks[2].shiftin) * 255.0 / masks[2].maxValue + 0.5;
			dest->pixels[i+j*dest->width].a = ((raw & masks[3].mask) >> masks[3].shiftin) * 255.0 / masks[3].maxValue + 0.5;

			offset += bytesPerPixel;
		}
		offset += paddingBytes;
	}

	return dest;
}

Bitmap* newBitmap(uint8_t* filedata, uint64_t size) {
	BmpFileHeader* fh = newFileHeader(filedata, size);
	BmpDib* dib = newBitmapDIB(filedata+14, size);
	Bitmap* bitmap = loadBmp(filedata+fh->pixelOffset, dib);

	free(fh);
	free(dib);

	return bitmap;
}

void freeBitmap(Bitmap* bitmap) {
	//memUtil.free2D((void**)bitmap->pixels, bitmap->height);
	free(bitmap->pixels);
	free(bitmap);
}

const BitmapManager manBitmap = {newBitmap, freeBitmap};
