#include "ObjLoader.h"

#include "util/DynamicArray.h"
#include "col/SAT.h"

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
						DynamicArray *vertices, DynamicArray *normals, DynamicArray *texCoords,
						DynamicArray *vIndices, DynamicArray *nIndices, DynamicArray *tIndices,
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
                        manDynamicArray.append(vertices, &coord);
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
                    	manDynamicArray.append(normals, &coord);
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
                    	manDynamicArray.append(texCoords, &coord);
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
                    	int va = v -1;
                    	manDynamicArray.append(vIndices, &va);
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
                            	int na = n -1;
                            	manDynamicArray.append(nIndices, &na);
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
                            	int ta = t -1;
                            	manDynamicArray.append(tIndices, &ta);
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
                                	int na = n -1;
                                	manDynamicArray.append(nIndices, &na);
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
    DynamicArray* vertices = manDynamicArray.new(100, sizeof(float));
    DynamicArray* normals = manDynamicArray.new(100, sizeof(float));
    DynamicArray* texCoords = manDynamicArray.new(100, sizeof(float));
    DynamicArray* vIndices = manDynamicArray.new(100, sizeof(int));
    DynamicArray* nIndices = manDynamicArray.new(100, sizeof(int));
    DynamicArray* tIndices = manDynamicArray.new(100, sizeof(int));

    int     vertexStride;
    int     normalStride;
    int     texCoordStride;
    int     vIndexStride;
    int     nIndexStride;
    int     tIndexStride;

    // Load information
    loadObj(filename, 
            vertices, normals, texCoords, vIndices, nIndices, tIndices,
            &vertexStride, &normalStride, &texCoordStride, &vIndexStride, &nIndexStride, &tIndexStride);

    // Number of faces (triangles or quads) in object = number of indices / number of indices per face.
    int numFaces = vIndices->size / vIndexStride;

    *numIndicesToDraw = vIndices->size;

    // Prepare OpenGL data structure
    DynamicArray    *verticesInternal = manDynamicArray.new(vertices->size, sizeof(struct Vertex_s));
    DynamicArray    *indicesInternal = manDynamicArray.new(vIndices->size, sizeof(unsigned int));

    // Initialize verticesInternal array
    struct Vertex_s *vvp = (struct Vertex_s *) calloc(1, sizeof(struct Vertex_s));
    for (int i = 0; i < vIndices->size; ++i) {
        manDynamicArray.append(verticesInternal, vvp);
    }
    struct Vertex_s *vp = (struct Vertex_s *) manDynamicArray.get(verticesInternal, 0);

    // Initialize indicesInternal array
    unsigned int *iip = (unsigned int *) calloc(1, sizeof(unsigned int));
    for (int i = 0; i < vIndices->size; ++i) {
        manDynamicArray.append(indicesInternal, iip);
    }
    unsigned int *ip = (unsigned int *) manDynamicArray.get(indicesInternal, 0);

    for (int j = 0, i = 0; i < numFaces; ++i) {
        // If object has normals, use normals
        if (normals->size > 0) {
            if (vertexStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).x = *(float*)manDynamicArray.get(vertices, *(int*)manDynamicArray.get(vIndices, i * vIndexStride) * vertexStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).y = *(float*)manDynamicArray.get(vertices, *(int*)manDynamicArray.get(vIndices, i * vIndexStride) * vertexStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).z = *(float*)manDynamicArray.get(vertices, *(int*)manDynamicArray.get(vIndices, i * vIndexStride) * vertexStride + 2);
            }
            if (normalStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nx = *(float*)manDynamicArray.get(normals, *(int*)manDynamicArray.get(nIndices, i * nIndexStride) * normalStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).ny = *(float*)manDynamicArray.get(normals, *(int*)manDynamicArray.get(nIndices, i * nIndexStride) * normalStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nz = *(float*)manDynamicArray.get(normals, *(int*)manDynamicArray.get(nIndices, i * nIndexStride) * normalStride + 2);
            }
            if (texCoordStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).u = *(float*)manDynamicArray.get(texCoords, *(int*)manDynamicArray.get(tIndices, i * tIndexStride) * texCoordStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).v = *(float*)manDynamicArray.get(texCoords, *(int*)manDynamicArray.get(tIndices, i * tIndexStride) * texCoordStride + 1);
            }
            (*(unsigned int *)manDynamicArray.get(indicesInternal, j)) = j;
            ++j;

            if (vertexStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).x = *(float*)manDynamicArray.get(vertices, *(int*)manDynamicArray.get(vIndices, i * vIndexStride + 1) * vertexStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).y = *(float*)manDynamicArray.get(vertices, *(int*)manDynamicArray.get(vIndices, i * vIndexStride + 1) * vertexStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).z = *(float*)manDynamicArray.get(vertices, *(int*)manDynamicArray.get(vIndices, i * vIndexStride + 1) * vertexStride + 2);
            }
            if (normalStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nx = *(float*)manDynamicArray.get(normals, *(int*)manDynamicArray.get(nIndices, i * nIndexStride + 1) * normalStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).ny = *(float*)manDynamicArray.get(normals, *(int*)manDynamicArray.get(nIndices, i * nIndexStride + 1) * normalStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nz = *(float*)manDynamicArray.get(normals, *(int*)manDynamicArray.get(nIndices, i * nIndexStride + 1) * normalStride + 2);
            }
            if (texCoordStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).u = *(float*)manDynamicArray.get(texCoords, *(int*)manDynamicArray.get(tIndices, i * tIndexStride + 1) * texCoordStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).v = *(float*)manDynamicArray.get(texCoords, *(int*)manDynamicArray.get(tIndices, i * tIndexStride + 1) * texCoordStride + 1);
            }
            (*(unsigned int *)manDynamicArray.get(indicesInternal, j)) = j;
            ++j;


            if (vertexStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).x = *(float*)manDynamicArray.get(vertices, *(int*)manDynamicArray.get(vIndices, i * vIndexStride + 2) * vertexStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).y = *(float*)manDynamicArray.get(vertices, *(int*)manDynamicArray.get(vIndices, i * vIndexStride + 2) * vertexStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).z = *(float*)manDynamicArray.get(vertices, *(int*)manDynamicArray.get(vIndices, i * vIndexStride + 2) * vertexStride + 2);
            }
            if (normalStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nx = *(float*)manDynamicArray.get(normals, *(int*)manDynamicArray.get(nIndices, i * nIndexStride + 2) * normalStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).ny = *(float*)manDynamicArray.get(normals, *(int*)manDynamicArray.get(nIndices, i * nIndexStride + 2) * normalStride + 1);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).nz = *(float*)manDynamicArray.get(normals, *(int*)manDynamicArray.get(nIndices, i * nIndexStride + 2) * normalStride + 2);
            }
            if (texCoordStride != 0) {
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).u = *(float*)manDynamicArray.get(texCoords, *(int*)manDynamicArray.get(tIndices, i * tIndexStride + 2) * texCoordStride);
                (*(struct Vertex_s *)manDynamicArray.get(verticesInternal, j)).v = *(float*)manDynamicArray.get(texCoords, *(int*)manDynamicArray.get(tIndices, i * tIndexStride + 2) * texCoordStride + 1);
            }
            (*(unsigned int *)manDynamicArray.get(indicesInternal, j)) = j;
            ++j;
        }
    }

    manVBO.setData(vbo, vp, vIndices->size*sizeof(struct Vertex_s), GL_STATIC_DRAW);

    manVBO.setData(positionVBO, vp, vIndices->size*sizeof(struct Vertex_s), GL_STATIC_DRAW);
    manVBO.setRenderInfo(positionVBO, vIndices->size, 3, sizeof(struct Vertex_s), 0);

    manVBO.setData(normalVBO, vp, vIndices->size*sizeof(struct Vertex_s), GL_STATIC_DRAW);
    manVBO.setRenderInfo(normalVBO, vIndices->size, 3, sizeof(struct Vertex_s), (char *)NULL + sizeof(float)*3);

    manVBO.setData(texCoordVBO, vp, vIndices->size*sizeof(struct Vertex_s), GL_STATIC_DRAW);
    manVBO.setRenderInfo(texCoordVBO, vIndices->size, 2, sizeof(struct Vertex_s), (char *)NULL + sizeof(float)*6);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices->size*sizeof(unsigned int), ip, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    manDynamicArray.delete(verticesInternal);
    manDynamicArray.delete(indicesInternal);
    manDynamicArray.delete(vertices);
    manDynamicArray.delete(normals);
    manDynamicArray.delete(texCoords);
    manDynamicArray.delete(vIndices);
    manDynamicArray.delete(nIndices);
    manDynamicArray.delete(tIndices);

    free(verticesInternal);
    free(indicesInternal);
    free(vertices);
    free(normals);
    free(texCoords);
    free(vIndices);
    free(nIndices);
    free(tIndices);

    free(vvp);
    free(iip);
}

static VAO *genVAOFromFile(const char *const filename, int vertexLocation, int normalLocation, int texcoordLocation) {
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
    manVAO.attachVBO(vao, positionVBO, vertexLocation, GL_FLOAT);

    // normal
    manVAO.attachVBO(vao, normalVBO, normalLocation, GL_FLOAT);

    // texCoord
    manVAO.attachVBO(vao, texCoordVBO, texcoordLocation, GL_FLOAT);

    free(vbo);
    free(positionVBO);
    free(normalVBO);
    free(texCoordVBO);

    return vao;
}

static PhysicsCollider* loadCollisionMesh(const char *const filename, Vec3* position, Vec3* rotation, Vec3* scale, Vec3* velocity) {
    // Setup data structures for receiving information
    DynamicArray* vertices = manDynamicArray.new(100, sizeof(float));
    DynamicArray* normals = manDynamicArray.new(100, sizeof(float));

    // Load information
    loadObj(filename, vertices, normals, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    DynamicArray* verts = manDynamicArray.new(vertices->size/3, sizeof(Vec3));
    DynamicArray* norms = manDynamicArray.new(normals->size/3, sizeof(Vec3));

    for(int i = 0; i < verts->capacity; i++) {
    	Vec3 vert = manVec3.create(NULL, *(float*)manDynamicArray.get(vertices, i*3), *(float*)manDynamicArray.get(vertices, i*3+1), *(float*)manDynamicArray.get(vertices, i*3+2));
    	manDynamicArray.append(verts, &vert);
    }

    for(int i = 0; i < norms->capacity; i++) {
    	Vec3 norm = manVec3.create(NULL, *(float*)manDynamicArray.get(normals, i*3), *(float*)manDynamicArray.get(normals, i*3+1), *(float*)manDynamicArray.get(normals, i*3+2));
    	manDynamicArray.append(norms, &norm);
    }

    DynamicArray* optiNorms = manDynamicArray.new(1, sizeof(Vec3));
    for(int i = 0; i < norms->size; i++) {
    	bool flag = true;
    	Vec3 norm1 = manVec3.create(NULL, *(float*)manDynamicArray.get(normals, i*3), *(float*)manDynamicArray.get(normals, i*3+1), *(float*)manDynamicArray.get(normals, i*3+2));

    	for(int j = 0; j < optiNorms->size; j++) {
        	Vec3 norm2 = manVec3.createFromVec3(NULL, (Vec3*)manDynamicArray.get(optiNorms, j));

        	if ((norm1.x == norm2.x) && (norm1.y == norm2.y) && (norm1.z == norm2.z)) {
        		flag = false;
        		break;
        	}

        	if ((norm1.x == -norm2.x) && (norm1.y == -norm2.y) && (norm1.z == -norm2.z)) {
            	flag = false;
            	break;
        	}
    	}

    	if (flag)
    		manDynamicArray.append(optiNorms, &norm1);
    }

    SATMesh tmpMesh;
    tmpMesh.nCount = optiNorms->size;
    tmpMesh.norms = (Vec3*)optiNorms->contents;
    tmpMesh.vCount = verts->size;
    tmpMesh.verts = (Vec3*)verts->contents;

    DynamicArray* optiVerts = manDynamicArray.new(1, sizeof(Vec3));
    int* minPoints = malloc(sizeof(int)*optiNorms->size);
    int* maxPoints = malloc(sizeof(int)*optiNorms->size);
    for(int i = 0; i < optiNorms->size; i++) {
    	SATProjection proj;
    	proj.axis = manVec3.createFromVec3(NULL, (Vec3*)manDynamicArray.get(optiNorms, i));
    	proj.max = SCALAR_MIN_VAL;
        proj.min = SCALAR_MAX_VAL;

       	sat.projectMesh(&proj, &tmpMesh);
       	bool flagMin = true;
       	bool flagMax = true;

       	for(int j = 0; j < optiVerts->size; j++) {
           	Vec3* vert = (Vec3*)manDynamicArray.get(optiVerts, j);

           	if ((vert->x == proj.pntMin.x) && (vert->y == proj.pntMin.y) && (vert->z == proj.pntMin.z)) {
           		flagMin = false;
           		minPoints[i] = j;
           	}

           	if ((vert->x == proj.pntMax.x) && (vert->y == proj.pntMax.y) && (vert->z == proj.pntMax.z)) {
           		flagMax = false;
           		maxPoints[i] = j;
           	}
       	}

       	if (flagMin) {
       		manDynamicArray.append(optiVerts, &proj.pntMin);
       		minPoints[i] = optiVerts->size-1;
       	}

       	if (flagMax) {
       		manDynamicArray.append(optiVerts, &proj.pntMax);
       		maxPoints[i] = optiVerts->size-1;
       	}
    }

    printf("[Collision Mesh Loader] Reduced normals by: %d\n", norms->size-optiNorms->size);
    printf("[Collision Mesh Loader] Reduced verts by: %d\n", verts->size-optiVerts->size);

    manDynamicArray.delete(verts);
    manDynamicArray.delete(norms);
    manDynamicArray.delete(vertices);
    manDynamicArray.delete(normals);

    Vec3 center = manVec3.create(NULL, 0,0,0);
    scalar radius = 0;
    for(int i = 0; i < optiVerts->size; i++) {
    	Vec3* vert = (Vec3*)manDynamicArray.get(optiVerts, i);
    	Vec3 dispVec = manVec3.sub(vert, &center);
    	scalar dist = manVec3.magnitude(&dispVec);
    	if (dist>radius)
    		radius = dist;
    }


    PhysicsCollider* result = manPhysCollider.new(position, rotation, scale, velocity);
    ColliderSimpleMesh* colMesh = manColMesh.newSimpleMesh(optiVerts->size, (Vec3*)optiVerts->contents, optiNorms->size, (Vec3*)optiNorms->contents, minPoints, maxPoints);
    result->nPhase.type = COL_TYPE_SIMPLE_MESH;
    manPhysCollider.attachNarrowphaseSimpleMesh(result, colMesh);
    manPhysCollider.setBroadphase(result, &center, radius);

    return result;
}



const ObjLoader objLoader = {loadObj, genVAOFromFile, loadCollisionMesh};
