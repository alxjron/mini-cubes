/**
 * File includes definitions for working on a mesh
 *
 */
#include "mesh.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>

Mesh* initMesh() {
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

    return result;
}

void setMeshVertPointer(Mesh* meshPtr, Vertex* vert, int size) {
    meshPtr->vertPointer = vert;
    meshPtr->vertSize = size;

    glBindVertexArray(meshPtr->vertArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, meshPtr->vertBufferObj);
    glBufferData(GL_ARRAY_BUFFER, meshPtr->vertSize * sizeof(Vertex), meshPtr->vertPointer, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
}

void setMeshElemPointer(Mesh* meshPtr, GLuint* elem, int size) {
    meshPtr->elementPointer = elem;
    meshPtr->elemSize = size;

    glBindVertexArray(meshPtr->vertArrayObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshPtr->elementBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshPtr->elemSize * sizeof(GLuint), meshPtr->elementPointer, GL_STATIC_DRAW);
}

void drawMesh(Mesh mesh) {
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glBindVertexArray(mesh.vertArrayObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.elementBufferObj);
    glDrawElements(GL_TRIANGLES, mesh.elemSize, GL_UNSIGNED_INT, 0);
}
