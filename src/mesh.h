#include <glad/glad.h>
#include <cglm/struct.h>

#ifndef MESH_H
#define MESH_H

typedef struct _vertex {
    GLfloat pos[3];
} Vertex;

typedef struct _mesh {
    vec3s position;

    GLuint vertArrayObj;
    GLuint vertBufferObj;
    GLuint elementBufferObj;

    int vertSize;
    Vertex* vertPointer;
    int elemSize;
    GLuint* elementPointer;
} Mesh;

enum CubeFace {
    FRONT, BACK,
    LEFT, RIGHT,
    TOP, BOTTOM
};

/**
 * Initialize mesh populating the object
 * IDs and return the mesh structure.
 *
 */
Mesh* initMesh(vec3s pos);

/**
 * Set the mesh pointers.
 *
 */
void setMeshVertPointer(Mesh* meshPtr, Vertex* vert, int size);
void setMeshElemPointer(Mesh* meshPtr, GLuint* elem, int size);

void clearMeshVertPointer(Mesh* meshPtr);
void clearMeshElemPointer(Mesh* meshPtr);

/**
 * Add a face to the mesh. 
 *
 */
void addFace(Mesh* mesh, enum CubeFace face, vec3s position, float scale);

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
