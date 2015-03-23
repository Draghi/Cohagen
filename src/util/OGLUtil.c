#include "graphics.h"

#include <math.h>
#include <limits.h>

/**
 * Helper function to enable backface culling
 *
 * @param windingDir The direction that the polygon winds in (GL_CW/ GL_CCE)
 */
static void setBackfaceCulling(const GLenum windingDir) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(windingDir);
}

static uint32_t getPixelSize(const GLenum type) {

	uint32_t componentCount = 0;
	uint32_t componentBits = 0;
	uint32_t component2Bits = 0;

	switch (type) {
		/*
		 * Single component
		 */
		case(GL_ALPHA):
		componentCount = 1;
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
		case(GL_RGB12):
		case(GL_RGBA12):
			componentBits = 12;
			break;

		/*
		 * 16 bit
		 */
		case(GL_RGB16):
		case(GL_RGBA16):
			componentBits = 16;
			break;


		/*
		 * Special Cases
		 */

		case(GL_RGB10_A2):
			componentCount = 3;
			componentBits = 10;
			component2Bits = 2;
			break;

		case(GL_RGB5_A1):
			componentCount = 3;
			componentBits = 5;
			component2Bits = 1;
			break;

	}

	return ceil((componentCount*componentBits+component2Bits)/((float)CHAR_BIT*sizeof(GLubyte)));
}

static bool checkError() {
	bool flag = false;
	while(true) {
		GLenum glErr = glGetError();

		switch(glErr) {
			case GL_NO_ERROR:
				if (!flag)
					cohLog.logGLError("GLError(%d) No error\n", glErr);
				return flag;

			case GL_INVALID_ENUM:
				cohLog.logGLError("GLError(%d) Invalid Enum\n", glErr);
				flag = true;
				break;

			case GL_INVALID_VALUE:
				cohLog.logGLError("GLError(%d) Invalid Value\n", glErr);
				flag = true;
				break;

			case GL_INVALID_OPERATION:
				cohLog.logGLError("GLError(%d) Invalid Operation\n", glErr);
				flag = true;
				break;

			case GL_STACK_OVERFLOW:
				cohLog.logGLError("GLError(%d) Stack Overflow\n", glErr);
				flag = true;
				break;

			case GL_STACK_UNDERFLOW:
				cohLog.logGLError("GLError(%d) Stack Underflow\n", glErr);
				flag = true;
				break;

			case GL_OUT_OF_MEMORY:
				cohLog.logGLError("GLError(%d) Out of memory\n", glErr);
				flag = true;
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				cohLog.logGLError("GLError(%d) Invalid Framebuffer operation\n", glErr);
				flag = true;
				break;

			default:
				cohLog.logGLError("GLError(%d) Unknown error occurred\n", glErr);
				flag = true;
				break;
		}
	}
}

const OGLUtilManager manOGLUtil = {setBackfaceCulling, getPixelSize, checkError};

