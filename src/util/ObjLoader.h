#ifndef UTIL_OBJLOADER_H
#define UTIL_OBJLOADER_H

#include <stdio.h>
#include <string.h>

#include "DynamicFloatArray.h"
#include "DynamicIntArray.h"

typedef struct ObjLoader_s {
    /**
     *  Loads an obj file.
     *
     *  @param  filename        pointer to char, path to file.
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
} ObjLoader;

/**
 * Expose singleton.
 */
extern const ObjLoader objLoader;

#endif