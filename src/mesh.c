/**
 * File includes definitions for working on a mesh
 *
 */
#include "mesh.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <cglm/struct.h>

Mesh* initMesh(vec3s pos) {
    Mesh* result = malloc(sizeof(Mesh));

    if (result == NULL) {
        fprintf(stderr, "ERROR: Cannot allocate space for Mesh!\n");
        exit(1);
    }

    // VAO
    glGenVertexArrays(1, &(result->vertArrayObj));
    glBindVertexArray(result->vertArrayObj);

    // VBO
    glGenBuffers(1, &(result->vertBufferObj));

    // EBO
    glGenBuffers(1, &(result->elementBufferObj));

    result->vertSize = 0;
    result->elemSize = 0;

    result->position = pos;

    return result;
}

void setMeshVertPointer(Mesh* meshPtr, Vertex* vert, int size) {
    // Free previous pointers
    if (meshPtr->vertSize != 0)
        free(meshPtr->vertPointer);

    meshPtr->vertPointer = vert;
    meshPtr->vertSize = size;

    glBindVertexArray(meshPtr->vertArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, meshPtr->vertBufferObj);
    glBufferData(GL_ARRAY_BUFFER, meshPtr->vertSize * sizeof(Vertex), meshPtr->vertPointer, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
}

void setMeshElemPointer(Mesh* meshPtr, GLuint* elem, int size) {
    // Free previous pointers
    if (meshPtr->elemSize != 0)
        free(meshPtr->elementPointer);

    meshPtr->elementPointer = elem;
    meshPtr->elemSize = size;

    glBindVertexArray(meshPtr->vertArrayObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshPtr->elementBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshPtr->elemSize * sizeof(GLuint), meshPtr->elementPointer, GL_STATIC_DRAW);
}

void clearMeshVertPointer(Mesh* meshPtr) {
    // Free previous pointers
    if (meshPtr->vertSize != 0)
        free(meshPtr->vertPointer);

    meshPtr->vertPointer = NULL;
    meshPtr->vertSize = 0;
}

void clearMeshElemPointer(Mesh* meshPtr) {
    // Free previous pointers
    if (meshPtr->elemSize != 0)
        free(meshPtr->elementPointer);

    meshPtr->elementPointer = NULL;
    meshPtr->elemSize = 0;
}

void drawMesh(Mesh mesh) {
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glBindVertexArray(mesh.vertArrayObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.elementBufferObj);
    glDrawElements(GL_TRIANGLES, mesh.elemSize, GL_UNSIGNED_INT, 0);
}

/**
 * Add a face to the mesh. 
 *
 */
void addFace(Mesh* mesh, enum CubeFace face, vec3s position, float scale) {
    // Need to create new lists to not override memory
    int newVertSize = mesh->vertSize + 4;
    Vertex* newVerts = malloc(sizeof(Vertex) * newVertSize);

    int newElemSize = mesh->elemSize + 6;
    GLuint* newElems = malloc(sizeof(GLuint) * newElemSize);

    if (newVerts == NULL || newElems == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate memory for program\n");
        exit(1);
    }

    // Make copies of previous data
    for (int i = 0; i < mesh->vertSize; i++) 
        newVerts[i] = mesh->vertPointer[i];

    for (int i = 0; i < mesh->elemSize; i++) 
        newElems[i] = mesh->elementPointer[i];

    int oldVertSize = mesh->vertSize;
    int oldElemSize = mesh->elemSize;

    // New elems will always be 
    // 0, 1, 3, 0, 3, 2
    // added with the old size
    const int indices[6] = {0, 1, 3, 0, 3, 2};

    for (int i = 0; i < 6; i++)
        newElems[i + oldElemSize] = indices[i] + oldVertSize;
    
    switch (face) {
        case 0:
            newVerts[oldVertSize]     = (Vertex) {.pos = {-scale + position.x, scale + position.y, scale + position.z}};
            newVerts[oldVertSize + 1] = (Vertex) {.pos = {scale + position.x, scale + position.y, scale + position.z}};
            newVerts[oldVertSize + 2] = (Vertex) {.pos = {-scale + position.x, -scale + position.y, scale + position.z}};
            newVerts[oldVertSize + 3] = (Vertex) {.pos = {scale + position.x, -scale + position.y, scale + position.z}};
            break;
        case 1:
            // Front but flipped and on -z side
            newVerts[oldVertSize]     = (Vertex) {.pos = {-scale + position.x, -scale + position.y, -scale + position.z}};
            newVerts[oldVertSize + 1] = (Vertex) {.pos = {scale + position.x, -scale + position.y, -scale + position.z}};
            newVerts[oldVertSize + 2] = (Vertex) {.pos = {-scale + position.x, scale + position.y, -scale + position.z}};
            newVerts[oldVertSize + 3] = (Vertex) {.pos = {scale + position.x, scale + position.y, -scale + position.z}};
            break;
        case 2:
            // Front but flipped and on -x side
            newVerts[oldVertSize]     = (Vertex) {.pos = {-scale + position.x, -scale + position.y, -scale + position.z}};
            newVerts[oldVertSize + 1] = (Vertex) {.pos = {-scale + position.x, scale + position.y, -scale + position.z}};
            newVerts[oldVertSize + 2] = (Vertex) {.pos = {-scale + position.x, -scale + position.y, scale + position.z}};
            newVerts[oldVertSize + 3] = (Vertex) {.pos = {-scale + position.x, scale + position.y, scale + position.z}};
            break;
        case 3:
            // Front but on +x side
            newVerts[oldVertSize]     = (Vertex) {.pos = {scale + position.x, -scale + position.y, scale + position.z}};
            newVerts[oldVertSize + 1] = (Vertex) {.pos = {scale + position.x, scale + position.y, scale + position.z}};
            newVerts[oldVertSize + 2] = (Vertex) {.pos = {scale + position.x, -scale + position.y, -scale + position.z}};
            newVerts[oldVertSize + 3] = (Vertex) {.pos = {scale + position.x, scale + position.y, -scale + position.z}};
            break;
        case 4:
            // Front but flipped and on +y side
            newVerts[oldVertSize]     = (Vertex) {.pos = {-scale + position.x, scale + position.y, -scale + position.z}};
            newVerts[oldVertSize + 1] = (Vertex) {.pos = {scale + position.x, scale + position.y, -scale + position.z}};
            newVerts[oldVertSize + 2] = (Vertex) {.pos = {-scale + position.x, scale + position.y, scale + position.z}};
            newVerts[oldVertSize + 3] = (Vertex) {.pos = {scale + position.x, scale + position.y, scale + position.z}};
            break;
        case 5:
            // Front but on -y side
            newVerts[oldVertSize]     = (Vertex) {.pos = {-scale + position.x, -scale + position.y, scale + position.z}};
            newVerts[oldVertSize + 1] = (Vertex) {.pos = {scale + position.x, -scale + position.y, scale + position.z}};
            newVerts[oldVertSize + 2] = (Vertex) {.pos = {-scale + position.x, -scale + position.y, -scale + position.z}};
            newVerts[oldVertSize + 3] = (Vertex) {.pos = {scale + position.x, -scale + position.y, -scale + position.z}};
            break;
    }

    // Set it to the new data
    setMeshVertPointer(mesh, newVerts, newVertSize);

    setMeshElemPointer(mesh, newElems, newElemSize);
}

void freeMesh(Mesh** meshPtrPtr) {
    Mesh* meshPtr = *meshPtrPtr;

    // VAO
    glDeleteVertexArrays(1, &(meshPtr->vertArrayObj));

    // VBO
    glDeleteBuffers(1, &(meshPtr->vertBufferObj));

    // EBO
    glDeleteBuffers(1, &(meshPtr->elementBufferObj));

    if (meshPtr->vertSize != 0)
        free(meshPtr->vertPointer);
    if (meshPtr->elemSize != 0)
        free(meshPtr->elementPointer);

    free(meshPtr);
    *meshPtrPtr = NULL;
}
