/*
 * GLUtil.h
 *
 *  Created on: 8 Mar 2015
 *      Author: draghi
 */

#ifndef GL_GLUTIL_H_
#define GL_GLUTIL_H_

#include "../graphics.h"

/**
 * Helper function to enable backface culling
 *
 * @param windingDir The direction that the polygon winds in (GL_CW/ GL_CCE)
 */
void glSetBackfaceCulling(GLenum windingDir);

#endif /* GL_GLUTIL_H_ */
