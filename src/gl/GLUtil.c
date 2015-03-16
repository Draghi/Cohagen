#include "../graphics.h"

#include <math.h>
#include <limits.h>

/**
 * Helper function to enable backface culling
 *
 * @param windingDir The direction that the polygon winds in (GL_CW/ GL_CCE)
 */
void glSetBackfaceCulling(const GLenum windingDir) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(windingDir);
}

uint32_t glGetPixelSize(const GLenum type) {

	uint32_t componentCount = 0;
	uint32_t componentBits = 0;
	uint32_t component2Bits = 0;

	switch (type) {
		/*
		 * Single component
		 */
		case(GL_ALPHA):
		case(GL_ALPHA4):
		case(GL_ALPHA8):
		case(GL_ALPHA12):
		case(GL_ALPHA16):
		case(GL_LUMINANCE):
		case(GL_LUMINANCE4):
		case(GL_LUMINANCE8):
		case(GL_LUMINANCE12):
		case(GL_LUMINANCE16):
		case(GL_INTENSITY):
		case(GL_INTENSITY4):
		case(GL_INTENSITY8):
		case(GL_INTENSITY12):
		case(GL_INTENSITY16):
		componentCount = 1;
		break;

		/*
		 * Double component
		 */
		case(GL_LUMINANCE4_ALPHA4):
		case(GL_LUMINANCE6_ALPHA2):
		case(GL_LUMINANCE8_ALPHA8):
		case(GL_LUMINANCE12_ALPHA4):
		case(GL_LUMINANCE12_ALPHA12):
		case(GL_LUMINANCE16_ALPHA16):
		componentCount = 2;
		break;

		/*
		 * Tri component
		 */
		case(GL_RGB):
		case(GL_RGB4):
		case(GL_RGB5):
		case(GL_RGB8):
		case(GL_RGB10):
		case(GL_RGB12):
		case(GL_RGB16):
		componentCount = 3;
		break;

		/*
		 * Quad component
		 */
		case(GL_RGBA):
		case(GL_RGBA2):
		case(GL_RGBA4):
		case(GL_RGB5_A1):
		case(GL_RGBA8):
		case(GL_RGB10_A2):
		case(GL_RGBA12):
		case(GL_RGBA16):
		componentCount = 4;
		break;
	}

	switch (type) {
		case(GL_ALPHA):
		case(GL_RGB):
		case(GL_RGBA):
		case(GL_LUMINANCE):
		case(GL_INTENSITY):
		break; /**@todo: Find out default*/

		/*
		 * 2 bit
		 */
		case(GL_RGBA2):
			componentBits = 2;
			break;

		/*
		 * 4 bit
		 */
		case(GL_ALPHA4):
		case(GL_LUMINANCE4):
		case(GL_INTENSITY4):
		case(GL_LUMINANCE4_ALPHA4):
		case(GL_RGB4):
		case(GL_RGBA4):
			componentBits = 4;
			break;

		/*
		 * 5 bit
		 */
		case(GL_RGB5):
			componentBits = 5;
			break;

		/*
		 * 8 bit
		 */
		case(GL_ALPHA8):
		case(GL_LUMINANCE8):
		case(GL_INTENSITY8):
		case(GL_LUMINANCE8_ALPHA8):
		case(GL_RGB8):
		case(GL_RGBA8):
			componentBits = 8;
			break;

		/*
		 * 10 bit
		 */
		case(GL_RGB10):
			componentBits = 10;
			break;

		/*
		 * 12 bit
		 */
		case(GL_ALPHA12):
		case(GL_LUMINANCE12):
		case(GL_LUMINANCE12_ALPHA12):
		case(GL_INTENSITY12):
		case(GL_RGB12):
		case(GL_RGBA12):
			componentBits = 12;
			break;

		/*
		 * 16 bit
		 */
		case(GL_ALPHA16):
		case(GL_LUMINANCE16):
		case(GL_LUMINANCE16_ALPHA16):
		case(GL_INTENSITY16):
		case(GL_RGB16):
		case(GL_RGBA16):
			componentBits = 16;
			break;


		/*
		 * Special Cases
		 */
		case(GL_LUMINANCE12_ALPHA4):
			componentCount = 1;
			componentBits = 12;
			component2Bits = 4;
			break;

		case(GL_RGB10_A2):
			componentCount = 3;
			componentBits = 10;
			component2Bits = 2;
			break;

		case(GL_LUMINANCE6_ALPHA2):
			componentCount = 6;
			componentBits = 6;
			component2Bits = 2;
			break;

		case(GL_RGB5_A1):
			componentCount = 3;
			componentBits = 5;
			component2Bits = 1;
			break;

	}

	return ceil((componentCount*componentBits+component2Bits)/(CHAR_BIT*sizeof(GLubyte)));
}
