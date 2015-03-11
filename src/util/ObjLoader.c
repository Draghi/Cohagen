#include "ObjLoader.h"

static void loadObj(    const char *const filename, 
                        DynamicFloatArray *vertices, DynamicFloatArray *normals, DynamicFloatArray *texCoords, 
                        DynamicIntArray *vIndices, DynamicIntArray *nIndices, DynamicIntArray *tIndices,
                        int *vertexStride, int *normalStride, int *texCoordStride,
                        int *vIndexStride, int *nIndexStride, int *tIndexStride) {
    /*
     *  Number of characters we expect to read from any single line in an .obj file.
     *  Realistically, shouldn't need to be too long.
     */
    const int MAX_LINE_LENGTH = 256;
    /*
     *  Max character size of any element(co-ordinate) in the obj file.
     */
    const int MAX_ELEMENT_SIZE = 32;

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
                        vIndices->append(vIndices, v);
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
                                nIndices->append(nIndices, n);
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
                                tIndices->append(tIndices, t);
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
                                    nIndices->append(nIndices, n);
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
        fprintf(stderr, "Can't open file '%s' for reading.", "cube.obj");
    }

    fclose(ifp);
}

const ObjLoader objLoader = {loadObj};