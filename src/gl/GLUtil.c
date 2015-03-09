#include "GLUtil.h"
#include "../graphics.h"

/**
 * Helper function to enable backface culling
 *
 * @param windingDir The direction that the polygon winds in (GL_CW/ GL_CCE)
 */
void glSetBackfaceCulling(GLenum windingDir) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(windingDir);
}
