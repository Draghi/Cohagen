#ifndef COH_GLUTIL_H
#define COH_GLUTIL_H

#include "../lib/ogl.h"

#include <stdbool.h>

struct OGLUtilManager_s {
	/**
	 * Helper function to enable backface culling
	 *
	 * @param windingDir The direction that the polygon winds in (GL_CW/ GL_CCE)
	 */
	void(* setBackfaceCulling)(const GLenum);

	/**
	 * Attempts to return the size in whole bytes of a pixel of a given type.
	 * (GL_RGB5 will be 2 bytes, despite only using 1.5 bytes)
	 * @param type The type to attempt to work out the size of.
	 * @return The size of the data type in whole bytes.
	 */
	uint32_t(* getPixelSize)(const GLenum);

	/**
	 * Checks the OpenGL error state and prints messages.
	 * @return Whether an error was returned.
	 */
	bool(* checkError)();
};

typedef struct OGLUtilManager_s OGLUtilManager;

extern const OGLUtilManager manOGLUtil;

#endif /* GL_GLUTIL_H_ */
