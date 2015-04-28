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
static void setupBuffers(VBO *vbo, GLuint ibo, const char *const filename, int *const numIndicesToDraw) {
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
    struct Vertex_s *vp = calloc(vIndices.size, sizeof(struct Vertex_s));
    for (int i = 0; i < vIndices.size; ++i) {
        manDynamicArray.append(verticesInternal, vp);
        ++vp;
    }
    // Get address of first element so we can "free" later.
    vp = manDynamicArray.get(verticesInternal, 0);

    // Initialize indicesInternal array
    unsigned int *ip = calloc(vIndices.size, sizeof(unsigned int));
    for (int i = 0; i < vIndices.size; ++i) {
        manDynamicArray.append(indicesInternal, ip);
        ++ip;
    }
    // Get address of first element so we can "free" later.
    ip = manDynamicArray.get(indicesInternal, 0);

    for (int j = 0, i = 0; i < numFaces; ++i) {
        // If object has normals, use normals
        if (normals.size > 0) {
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).x = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride) * vertexStride);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).y = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride) * vertexStride + 1);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).z = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride) * vertexStride + 2);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nx = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride) * normalStride);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).ny = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride) * normalStride + 1);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nz = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride) * normalStride + 2);
            (*(unsigned int *)manDynamicArray.get(indicesInternal, j)) = j;
            ++j;


            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).x = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 1) * vertexStride);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).y = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 1) * vertexStride + 1);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).z = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 1) * vertexStride + 2);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nx = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 1) * normalStride);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).ny = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 1) * normalStride + 1);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nz = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 1) * normalStride + 2);
            (*(unsigned int *)manDynamicArray.get(indicesInternal, j)) = j;
            ++j;


            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).x = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 2) * vertexStride);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).y = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 2) * vertexStride + 1);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).z = vertices.get(&vertices, vIndices.get(&vIndices, i * vIndexStride + 2) * vertexStride + 2);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nx = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 2) * normalStride);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).ny = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 2) * normalStride + 1);
            (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nz = normals.get(&normals, nIndices.get(&nIndices, i * nIndexStride + 2) * normalStride + 2);
            (*(unsigned int *)manDynamicArray.get(indicesInternal, j)) = j;
            ++j;
        }
    }

    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferData(GL_ARRAY_BUFFER, vIndices.size*sizeof(struct Vertex_s), vp, GL_STATIC_DRAW);
    
    manVBO.setData(vbo, vp, vIndices.size*sizeof(struct Vertex_s), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size*sizeof(unsigned int), ip, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(vp);
    free(ip);
    manDynamicArray.delete(verticesInternal);
    manDynamicArray.delete(indicesInternal);

    deleteDynamicFloatArray(&vertices);
    deleteDynamicFloatArray(&normals);
    deleteDynamicFloatArray(&texCoords);
    deleteDynamicIntArray(&vIndices);
    deleteDynamicIntArray(&nIndices);
    deleteDynamicIntArray(&tIndices);
}

static GLuint genVAOFromFile(const char *const filename, int *const numIndicesToDraw) {
    // VBO     *vbo = vboManager.createVBO();
    // GLuint  ibo;
    // VAO     *vao = vaoManager.newVAO();

    // glGenBuffers(1, &ibo);

    // // setupBuffers(vbo, ibo);

    // vaoManager.bind(vao);
    // vboManager.bind(vbo);

    // GLuint vao, vbo, ibo;
    GLuint vao, ibo;

    // Generate vbo and ibo buffers
    // glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    // Fill them with data
    VBO *vbo = manVBO.new();
    setupBuffers(vbo, ibo, filename, numIndicesToDraw);

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    manVBO.bind(vbo);

    // Let VAO know where data is in vbo
    // position    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_s), 0);

    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_s), (char *)NULL + sizeof(float)*3);

    // texCoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_s), (char *)NULL + sizeof(float)*6);

    // Bind index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBindVertexArray(0);

    return vao;
}

const ObjLoader objLoader = {loadObj, genVAOFromFile};
