/*
 * GLUtil.h
 *
 *  Created on: 8 Mar 2015
 *      Author: draghi
 */

#ifndef COH_GLUTIL_H
#define COH_GLUTIL_H

#include "../graphics.h"

/**
 * Helper function to enable backface culling
 *
 * @param windingDir The direction that the polygon winds in (GL_CW/ GL_CCE)
 */
void glSetBackfaceCulling(const GLenum windingDir);
uint32_t glGetPixelSize(const GLenum type);

#endif /* GL_GLUTIL_H_ */
