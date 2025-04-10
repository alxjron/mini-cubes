#include <glad/glad.h>

#ifndef MESH_H
#define MESH_H

typedef struct _vertex {
    GLfloat pos[3];
} Vertex;

typedef struct _mesh {
    GLuint vertArrayObj;
    GLuint vertBufferObj;
    GLuint elementBufferObj;

    int vertSize;
    Vertex* vertPointer;
    int elemSize;
    GLuint* elementPointer;
} Mesh;

/**
 * Initialize mesh populating the object
 * IDs and return the mesh structure.
 *
 */
Mesh* initMesh();

/**
 * Set the mesh pointers.
 *
 */
void setMeshVertPointer(Mesh* meshPtr, Vertex* vert, int size);
void setMeshElemPointer(Mesh* meshPtr, GLuint* elem, int size);

/**
 * Draw whatever is in the mesh.
 *
 */
void drawMesh(Mesh mesh);

/**
 * Free everything in the mesh and the
 * mesh itself.
 *
 */
void freeMesh(Mesh** meshPtrPtr);

#endif
