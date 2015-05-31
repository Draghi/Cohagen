#ifndef UTIL_OBJLOADER_H
#define UTIL_OBJLOADER_H

#include "col/PhysicsCollider.h"
#include <stdio.h>
#include <string.h>

#include "util/DynamicFloatArray.h"
#include "util/DynamicIntArray.h"
#include "gl/VAO.h"
#include "gl/VBO.h"
#include "lib/ogl.h"
#include "gl/EAB.h"

/**
 *  Singleton used for loading .obj files.
 */
typedef struct ObjLoader_s {
    /**
     *  Loads an obj file. NULL can be passed to parameters not needed.
     *  Obj file MUST have a newline at the end of the file.
     *
     *  @param  filename        const pointer to const char, path to file.
     *  @param  vertices        pointer to DynamicFloatArray, array will hold all vertex data after function completes.
     *  @param  normals         pointer to DynamicFloatArray, array will hold all normal data after function completes.
     *  @param  texCoords       pointer to DynamicFloatArray, array will hold all texture co-ordinate data after function completes.
     *  @param  vIndices        pointer to DynamicIntArray, array will hold all vertex index data after function completes.
     *  @param  nIndices        pointer to DynamicIntArray, array will hold all normal index data after function completes.
     *  @param  tIndices        pointer to DynamicIntArray, array will hold all texture index data after function completes.
     *  @param  vertexStride    pointer to int, after function completes, will hold the number of floats per vertex.
     *  @param  normalStride    pointer to int, after function completes, will hold the number of floats per normal.
     *  @param  texCoordStride  pointer to int, after function completes, will hold the number of floats per texCoord.
     *  @param  vIndexStride    pointer to int, after function completes, will hold the number of vertex indices per face
     *                          (essentially the number of vertices per face, 3 for triangulated faces).
     *  @param  nIndexStride    pointer to int, after function completes, will hold the number of normal indices per face.
     *  @param  tIndexStride    pointer to int, after function completes, will hold the number of textureCoord indices per face.
     */
    void (*loadObj)(    const char *const filename, 
                        DynamicFloatArray *vertices, DynamicFloatArray *normals, DynamicFloatArray *texCoords, 
                        DynamicIntArray *vIndices, DynamicIntArray *nIndices, DynamicIntArray *tIndices,
                        int *vertexStride, int *normalStride, int *texCoordStride,
                        int *vIndexStride, int *nIndexStride, int *tIndexStride);
    /**
     *  Loads an obj file, sets up the appropriate VBOs and returns a 'ready-to-go' VAO.
     *
     *  @param  filename            const pointer to const char, path to file.
     *  @param  numIndicesToDraw    const pointer to int, when function returns, will contain the number
     *                              of indices that must be drawn to draw the object.
     *  @return                     pointer to VAO, VAO generated.
     */
    VAO *(*genVAOFromFile)(const char *const filename, int vertexLocation, int normalLocation, int texcoordLocation);

    /**
     * Loads a simple collision mesh from a obj file.
     * Calculates the broadphase params as well.
     *
     * @param filename const pointer to const char, path to file.
     * @param position The vec3 to be used for positioning.
     * @param rotation The vec3 to be used for rotation.
     * @param scale The vec3 to be used for scaling.
     * @param velocity The vec3 to be used for velocity.
     * @return
     */
    PhysicsCollider*(* loadCollisionMesh)(const char *const filename, Vec3* position, Vec3* rotation, Vec3* scale, Vec3* velocity);

} ObjLoader;

/**
 * Expose singleton.
 */
extern const ObjLoader objLoader;

/**
 *  Internal representation of a vertex in OpenGL.
 */
typedef struct Vertex_s {
    GLfloat     x, y, z;
    GLfloat     nx, ny, nz;
    GLfloat     u, v;
    GLfloat     padding[4];
} GLVertex;

#endif
