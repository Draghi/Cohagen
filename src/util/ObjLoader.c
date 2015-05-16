#include "ObjLoader.h"

#include "util/DynamicArray.h"

/*
 *  Number of characters we expect to read from any single line in an .obj file.
 *  Realistically, shouldn't need to be too long.
 */
static const int MAX_LINE_LENGTH = 256;
/*
 *  Max character size of any element(co-ordinate) in the obj file.
 */
static const int MAX_ELEMENT_SIZE = 32;

static void loadObj(    const char *const filename, 
                        DynamicFloatArray *vertices, DynamicFloatArray *normals, DynamicFloatArray *texCoords, 
                        DynamicIntArray *vIndices, DynamicIntArray *nIndices, DynamicIntArray *tIndices,
                        int *vertexStride, int *normalStride, int *texCoordStride,
                        int *vIndexStride, int *nIndexStride, int *tIndexStride) {
    FILE *ifp;

    // Initialize strides
    if (vertexStride != NULL) {
        *vertexStride = 0;
    }
    if (normalStride != NULL) {
        *normalStride = 0;
    }
    if (texCoordStride != NULL) {
        *texCoordStride = 0;
    }
    if (vIndexStride != NULL) {
        *vIndexStride = 0;
    }
    if (nIndexStride != NULL) {
        *nIndexStride = 0;
    }
    if (tIndexStride != NULL) {
        *tIndexStride = 0;
    }

    // Open file for reading.
    ifp = fopen(filename, "r");

    // If we can read file...
    if (ifp != NULL) {
        char line[MAX_LINE_LENGTH];

        while (fgets(line, MAX_LINE_LENGTH, ifp) != NULL) {
            char *handle = line;
            char key[3];

            sscanf(handle, "%s", key);

            // Advance handle into string
            for(char *cp = key; *cp != '\0'; ++cp) {
                ++handle;
            }

            if (strcmp(key, "v\0\0") == 0) {
                char element[MAX_ELEMENT_SIZE];

                // Count number of coordinates per vertex
                if (vertexStride != NULL) {
                    *vertexStride = 0;
                }
                do {
                    sscanf(handle, "%s", element);

                    float coord;
                    sscanf(element, "%f", &coord);

                    if (vertices != NULL) {
                        vertices->append(vertices, coord);
                    }

                    // Advance handle into string
                    for (char *cp = element; *cp != '\0'; ++cp) {
                        ++handle;
                    }

                    // Ensure handle is moved to the whitespace after the number just read
                    ++handle;

                    if (vertexStride != NULL) {
                        ++(*vertexStride);
                    }
                } while (*handle != '\n');
            }

            if (strcmp(key, "vn\0") == 0) {
                char element[MAX_ELEMENT_SIZE];

                // Count number of coordinates per normal
                if (normalStride != NULL) {
                    *normalStride = 0;
                }

                do {
                    sscanf(handle, "%s", element);

                    float coord;
                    sscanf(element, "%f", &coord);

                    if (normals != NULL) {
                        normals->append(normals, coord);
                    }

                    // Advance handle into string
                    for (char *cp = element; *cp != '\0'; ++cp) {
                        ++handle;
                    }

                    // Ensure handle is moved to the whitespace after the number just read
                    ++handle;

                    if (normalStride != NULL) {
                        ++(*normalStride);
                    }
                } while (*handle != '\n');
            }
            if (strcmp(key, "vt\0") == 0) {
                char element[MAX_ELEMENT_SIZE];

                // Count texture coordinate stride
                if (texCoordStride != NULL) {
                    (*texCoordStride) = 0;
                }
                do {
                    sscanf(handle, "%s", element);

                    float coord;
                    sscanf(element, "%f", &coord);

                    if (texCoords != NULL) {
                        texCoords->append(texCoords, coord);
                    }

                    // Advance handle into string
                    for (char *cp = element; *cp != '\0'; ++cp) {
                        ++handle;
                    }

                    // Ensure handle is moved to the whitespace after the number just read
                    ++handle;

                    if (texCoordStride != NULL) {
                        (*texCoordStride)++;
                    }
                } while (*handle != '\n');
            }
            if (strcmp(key, "f\0\0") == 0) {
                char element[MAX_ELEMENT_SIZE];

                int v, n, t;

                if (vIndexStride != NULL) {
                    *vIndexStride = 0;
                }
                if (nIndexStride != NULL) {
                    *nIndexStride = 0;
                }
                if (tIndexStride != NULL) {
                    *tIndexStride = 0;
                }
                do {
                    sscanf(handle, "%s", element);
                    
                    char *subHandle = element;

                    sscanf(subHandle, "%d", &v);

                    if (vIndices != NULL) {
                        vIndices->append(vIndices, v - 1);
                    }

                    // Count number of vertices per face
                    if (vIndexStride != NULL) {
                        ++(*vIndexStride);
                    }

                    // Advance sub-handle to next sub-element of face
                    while (*subHandle != '/' && *subHandle != '\0') {
                        ++subHandle;
                    }

                    /*
                     * If element is '/', next element is either a tex-coord index or
                     * a '/'
                     */
                    if (*subHandle == '/') {
                        // Move to next character
                        ++subHandle;

                        // If encountered '/' again, next element is a normal index.
                        if (*subHandle == '/') {
                            // Move past '/'
                            ++subHandle;

                            // Read normal index
                            sscanf(subHandle, "%d", &n);

                            if (nIndices != NULL) {
                                nIndices->append(nIndices, n - 1);
                            }

                            if (nIndexStride != NULL) {
                                ++(*nIndexStride);
                            }

                            // Advance sub-handle to next sub-element of face
                            while (*subHandle != '/' && *subHandle != '\0') {
                                ++subHandle;
                            }
                        } 
                        // If not encoutered '/' yet, next element must be tex-coord.
                        else {
                            // Read tex-coord index
                            sscanf(subHandle, "%d", &t);

                            if (tIndices != NULL) {
                                tIndices->append(tIndices, t - 1);
                            }

                            if (tIndexStride != NULL) {
                                ++(*tIndexStride);
                            }

                            // Advance sub-handle to next sub-element of face
                            while (*subHandle != '/' && *subHandle != '\0') {
                                ++subHandle;
                            }

                            // Check for and read normal index if existing.
                            if (*subHandle == '/') {
                                // Move past '/'
                                ++subHandle;

                                // Read normal index
                                sscanf(subHandle, "%d", &n);

                                if (nIndices != NULL) {
                                    nIndices->append(nIndices, n - 1);
                                }

                                if (nIndexStride != NULL) {
                                    ++(*nIndexStride);
                                }

                                // Advance sub-handle to next sub-element of face
                                while (*subHandle != '/' && *subHandle != '\0') {
                                    ++subHandle;
                                }
                            }
                        }
                    }

                    // Advance handle into string
                    for (char *cp = element; *cp != '\0'; ++cp) {
                        ++handle;
                    }

                    // Ensure handle is moved to the whitespace after the number just read
                    ++handle;
                } while (*handle != '\n');
            }
        }
    } else {
        fprintf(stderr, "Can't open file '%s' for reading.", filename);
    }

    fclose(ifp);
}

/*
 *  Fill vertex buffer object and index buffer object with data from file.
 */
static void setupBuffers(VBO *vbo, GLuint ibo, VBO *positionVBO, VBO *normalVBO, VBO *texCoordVBO, const char *const filename, int *const numIndicesToDraw) {
    // Setup data structures for receiving information
    DynamicFloatArray   vertices;
    DynamicFloatArray   normals;
    DynamicFloatArray   texCoords;
    DynamicIntArray     vIndices;
    DynamicIntArray     nIndices;
    DynamicIntArray     tIndices;

    newDynamicFloatArray(&vertices, 100);
    newDynamicFloatArray(&normals, 100);
    newDynamicFloatArray(&texCoords, 100);
    newDynamicIntArray(&vIndices, 100);
    newDynamicIntArray(&nIndices, 100);
    newDynamicIntArray(&tIndices, 100);

    int     vertexStride;
    int     normalStride;
    int     texCoordStride;
    int     vIndexStride;
    int     nIndexStride;
    int     tIndexStride;

    // Load information
    loadObj(filename, 
            &vertices, &normals, &texCoords, &vIndices, &nIndices, &tIndices,
            &vertexStride, &normalStride, &texCoordStride, &vIndexStride, &nIndexStride, &tIndexStride);

    // Number of faces (triangles or quads) in object = number of indices / number of indices per face.
    int numFaces = vIndices.size / vIndexStride;

    *numIndicesToDraw = vIndices.size;

    // Prepare OpenGL data structure
    DynamicArray    *verticesInternal = manDynamicArray.new(vertices.size, sizeof(struct Vertex_s));
    DynamicArray    *indicesInternal = manDynamicArray.new(vIndices.size, sizeof(unsigned int));

    // Initialize verticesInternal array
    struct Vertex_s *vvp = (struct Vertex_s *) calloc(1, sizeof(struct Vertex_s));
    for (int i = 0; i < vIndices.size; ++i) {
        manDynamicArray.append(verticesInternal, vvp);
    }
    struct Vertex_s *vp = (struct Vertex_s *) manDynamicArray.get(verticesInternal, 0);

    // Initialize indicesInternal array
    unsigned int *iip = (unsigned int *) calloc(1, sizeof(unsigned int));
    for (int i = 0; i < vIndices.size; ++i) {
        manDynamicArray.append(indicesInternal, iip);
    }
    unsigned int *ip = (unsigned int *) manDynamicArray.get(indicesInternal, 0);

    for (int j = 0, i = 0; i < numFaces; ++i) {
        // If object has normals, use normals
        if (normals.size > 0) {
            if (vertexStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).x = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride) * vertexStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).y = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride) * vertexStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).z = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride) * vertexStride + 2);
            }
            if (normalStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nx = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride) * normalStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).ny = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride) * normalStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nz = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride) * normalStride + 2);
            }
            if (texCoordStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).u = texCoords.get(&texCoords, tIndices.get(&tIndices, i * tIndexStride) * texCoordStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).v = texCoords.get(&texCoords, tIndices.get(&tIndices, i * tIndexStride) * texCoordStride + 1);
            }
            (*(unsigned int *)manDynamicArray.get(indicesInternal, j)) = j;
            ++j;

            if (vertexStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).x = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 1) * vertexStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).y = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 1) * vertexStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).z = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 1) * vertexStride + 2);
            }
            if (normalStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nx = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 1) * normalStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).ny = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 1) * normalStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nz = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 1) * normalStride + 2);
            }
            if (texCoordStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).u = texCoords.get(&texCoords, tIndices.get(&tIndices, i * tIndexStride + 1) * texCoordStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).v = texCoords.get(&texCoords, tIndices.get(&tIndices, i * tIndexStride + 1) * texCoordStride + 1);
            }
            (*(unsigned int *)manDynamicArray.get(indicesInternal, j)) = j;
            ++j;


            if (vertexStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).x = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 2) * vertexStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).y = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 2) * vertexStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).z = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 2) * vertexStride + 2);
            }
            if (normalStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nx = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 2) * normalStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).ny = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 2) * normalStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nz = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 2) * normalStride + 2);
            }
            if (texCoordStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).u = texCoords.get(&texCoords, tIndices.get(&tIndices, i * tIndexStride + 2) * texCoordStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).v = texCoords.get(&texCoords, tIndices.get(&tIndices, i * tIndexStride + 2) * texCoordStride + 1);
            }
            (*(unsigned int *)manDynamicArray.get(indicesInternal, j)) = j;
            ++j;
        }
    }

    manVBO.setData(vbo, vp, vIndices.size*sizeof(struct Vertex_s), GL_STATIC_DRAW);

    manVBO.setData(positionVBO, vp, vIndices.size*sizeof(struct Vertex_s), GL_STATIC_DRAW);
    manVBO.setRenderInfo(positionVBO, vIndices.size, 3, sizeof(struct Vertex_s), 0);

    manVBO.setData(normalVBO, vp, vIndices.size*sizeof(struct Vertex_s), GL_STATIC_DRAW);
    manVBO.setRenderInfo(normalVBO, vIndices.size, 3, sizeof(struct Vertex_s), (char *)NULL + sizeof(float)*3);

    manVBO.setData(texCoordVBO, vp, vIndices.size*sizeof(struct Vertex_s), GL_STATIC_DRAW);
    manVBO.setRenderInfo(texCoordVBO, vIndices.size, 2, sizeof(struct Vertex_s), (char *)NULL + sizeof(float)*6);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size*sizeof(unsigned int), ip, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    manDynamicArray.delete(verticesInternal);
    manDynamicArray.delete(indicesInternal);
    free(verticesInternal);
    free(indicesInternal);
    free(vvp);
    free(iip);
    
    deleteDynamicFloatArray(&vertices);
    deleteDynamicFloatArray(&normals);
    deleteDynamicFloatArray(&texCoords);
    deleteDynamicIntArray(&vIndices);
    deleteDynamicIntArray(&nIndices);
    deleteDynamicIntArray(&tIndices);
}

static VAO *genVAOFromFile(const char *const filename) {
    // GLuint vao, ibo;
    GLuint ibo;
    VAO *vao = manVAO.new();
    VBO *vbo = manVBO.new();
    VBO *positionVBO = manVBO.new();
    VBO *normalVBO = manVBO.new();
    VBO *texCoordVBO = manVBO.new();
    int numIndicesToDraw;

    // Generate ibo buffer
    glGenBuffers(1, &ibo);

    // Fill buffers with data
    setupBuffers(vbo, ibo, positionVBO, normalVBO, texCoordVBO, filename, &numIndicesToDraw);

    manVAO.setRenderInfo(vao, numIndicesToDraw);

    // Let VAO know where data is in vbo
    // position
    manVAO.attachVBO(vao, positionVBO, 0, GL_FLOAT);    

    // normal
    manVAO.attachVBO(vao, normalVBO, 1, GL_FLOAT);

    // texCoord
    manVAO.attachVBO(vao, texCoordVBO, 2, GL_FLOAT);

    free(vbo);
    free(positionVBO);
    free(normalVBO);
    free(texCoordVBO);

    return vao;
}

const ObjLoader objLoader = {loadObj, genVAOFromFile};
