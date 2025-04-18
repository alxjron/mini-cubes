/**
 * Contains functions for Regions
 * which are big cubes that hold
 * smaller cubes either Cubes or
 * Mini Cubes.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "region.h"

const int REGION_CUBE_DEPTH = 16;
const int REGION_MCUBE_DEPTH = 32;

char* ERR_CUBE = "ERROR";
char* AIR_CUBE = "";

Region* initRegion(vec3s pos) {
    Region* result = malloc(sizeof(Region));

    if (result == NULL) {
        fprintf(stderr, "ERROR: Cannot allocate space for Region!\n");
        exit(1);
    }

    // Initalize as air by default
    result->data = malloc(strlen(AIR_CUBE) + 1);
    result->data[0] = AIR_CUBE;

    result->regType = FILLED;

    result->meshPtr = initMesh(pos);
    
    // Define the neighbors
    result->up = NULL;
    result->down = NULL;
    result->left = NULL;
    result->right = NULL;
    result->front = NULL;
    result->back = NULL;

    return result;
}

int fillRegion(char* cubeID, Region* regPtr) {
    if (regPtr->regType == FILLED && strcmp(*(regPtr->data), cubeID) == 0) {
        return 0;
    }

    regPtr->regType = FILLED;

    // Only 1 string list
    char** newData = malloc(sizeof(char*));
    newData[0] = cubeID;

    free(regPtr->data);
    regPtr->data = newData;

    // update ourselves
    updateRegionMesh(regPtr);

    // update all neighbors
    updateRegionMesh(regPtr->up);
    updateRegionMesh(regPtr->down);
    updateRegionMesh(regPtr->left);
    updateRegionMesh(regPtr->right);
    updateRegionMesh(regPtr->front);
    updateRegionMesh(regPtr->back);

    return 1;
}

int setMCube(char* cubeID, Region* regPtr, vec3s pos) {
    int x = floorf(pos.x);
    int y = floorf(pos.y);
    int z = floorf(pos.z);

    char* mcubeAtPos = getMCube(regPtr, x, y, z);

    // If there is already a mini cube there
    // of the same ID
    if (strcmp(mcubeAtPos, cubeID) == 0) 
        return 0;

    // Need to convert from filled to
    // mcubed
    if (regPtr->regType == FILLED) {
        char* fillCubeID = *(regPtr->data);

        // list of all minicubes
        char** newData = malloc(REGION_MCUBE_DEPTH * REGION_MCUBE_DEPTH * REGION_MCUBE_DEPTH * sizeof(char*));

        for (int i = 0; i < REGION_MCUBE_DEPTH * REGION_MCUBE_DEPTH * REGION_MCUBE_DEPTH; i++)
            newData[i] = fillCubeID;

        free(regPtr->data);
        regPtr->data = newData;
        regPtr->regType = MCUBED;
    }
    // Need to convert from cubed to
    // mcubed
    else if (regPtr->regType == CUBED) {
    }

    // Set the value
    regPtr->data[x + z * REGION_MCUBE_DEPTH + y * REGION_MCUBE_DEPTH * REGION_MCUBE_DEPTH] = cubeID;

    // update ourselves
    updateRegionMesh(regPtr);

    // update all neighbors
    updateRegionMesh(regPtr->up);
    updateRegionMesh(regPtr->down);
    updateRegionMesh(regPtr->left);
    updateRegionMesh(regPtr->right);
    updateRegionMesh(regPtr->front);
    updateRegionMesh(regPtr->back);

    return 1;
}

void updateRegionMesh(Region* reg) {
    if (reg == NULL) 
        return;

    clearMeshVertPointer(reg->meshPtr);
    clearMeshElemPointer(reg->meshPtr);

    int x, y, z;

    for (y = 0; y < REGION_MCUBE_DEPTH; y++) {
        for (x = 0; x < REGION_MCUBE_DEPTH; x++) {
            for (z = 0; z < REGION_MCUBE_DEPTH; z++) {
                float rx, ry, rz;
                rx = (0.25f * (float) x) + reg->meshPtr->position.x;
                ry = (0.25f * (float) y) + reg->meshPtr->position.y;
                rz = (0.25f * (float) z) + reg->meshPtr->position.z;
                vec3s pos = {.x = rx, .y = ry, .z = rz};

                // If the current cube is air
                if (strcmp(getMCube(reg, x, y, z), AIR_CUBE) == 0) 
                    continue;

                // Top is air
                if (strcmp(getMCube(reg, x, y + 1, z), AIR_CUBE) == 0)
                    addFace(reg->meshPtr, TOP, pos, 0.25f);
                // Bottom is air
                if (strcmp(getMCube(reg, x, y - 1, z), AIR_CUBE) == 0)
                    addFace(reg->meshPtr, BOTTOM, pos, 0.25f);
                // Front is air
                if (strcmp(getMCube(reg, x, y, z + 1), AIR_CUBE) == 0) 
                    addFace(reg->meshPtr, FRONT, pos, 0.25f);
                // Back is air
                if (strcmp(getMCube(reg, x, y, z - 1), AIR_CUBE) == 0)
                    addFace(reg->meshPtr, BACK, pos, 0.25f);
                // Left is air
                if (strcmp(getMCube(reg, x - 1, y, z), AIR_CUBE) == 0)
                    addFace(reg->meshPtr, LEFT, pos, 0.25f);
                // Right is air
                if (strcmp(getMCube(reg, x + 1, y, z), AIR_CUBE) == 0)
                    addFace(reg->meshPtr, RIGHT, pos, 0.25f);
            }
        }
    }
}

int connectRegions(Region* src, Region* dest, enum CubeFace face) {
    if (src == NULL || dest == NULL)
        return 0;

    switch (face) {
        case FRONT:
            if (src->front != NULL || dest->back != NULL)
                return 0;
            src->front = dest;
            dest->back = src;
            return 1; 
        case BACK:
            if (src->back != NULL || dest->front != NULL)
                return 0;
            src->back = dest;
            dest->front = src;
            return 1;
        case LEFT:
            if (src->left != NULL || dest->right != NULL)
                return 0;
            src->left = dest;
            dest->right = src;
            return 1;
        case RIGHT:
            if (src->right != NULL || dest->left != NULL)
                return 0;
            src->right = dest;
            dest->left = src;
            return 1;
        case TOP:
            if (src->up != NULL || dest->down != NULL)
                return 0;
            src->up = dest;
            dest->down = src;
            return 1;
        case BOTTOM:
            if (src->down != NULL || dest->up != NULL)
                return 0;
            src->down = dest;
            dest->up = src;
            return 1;
    }
    return 0;
}

int detachRegions(Region* src, Region* reg) {
    if (src == NULL || reg == NULL)
        return 0;

    enum CubeFace face;

    if (src->front == reg)
        face = FRONT;
    else if (src->back == reg)
        face = BACK;
    else if (src->left == reg)
        face = LEFT;
    else if (src->right == reg)
        face = RIGHT;
    else if (src->up == reg)
        face = TOP;
    else if (src->down == reg)
        face = BOTTOM;
    else 
        return 0;

    switch (face) {
        case FRONT:
            src->front = NULL;
            reg->back = NULL;
            return 1; 
        case BACK:
            src->back = NULL;
            reg->front = NULL;
            return 1;
        case LEFT:
            src->left = NULL;
            reg->right = NULL;
            return 1;
        case RIGHT:
            src->right = NULL;
            reg->left = NULL;
            return 1;
        case TOP:
            src->up = NULL;
            reg->down = NULL;
            return 1;
        case BOTTOM:
            src->down = NULL;
            reg->up = NULL;
            return 1;
    }

    return 0;
}

char* getMCubeHelper(Region* reg, int x, int y, int z, int iter);

char* getMCube(Region* reg, int x, int y, int z) {
    return getMCubeHelper(reg, x, y, z, 0);
}

/**
 * A simple little helper that makes sure
 * getMCube does not recurse up to an
 * abitrary number
 *
 */
char* getMCubeHelper(Region* reg, int x, int y, int z, int iter) {
    // You've gone too far!
    if (iter == 10) {
        fprintf(stderr, "WARN: getMCube function has recursed too much!\n");
        return ERR_CUBE;
    }

    if (reg == NULL)
        return AIR_CUBE;

    // X
    if (x >= REGION_MCUBE_DEPTH) {
        return getMCubeHelper(reg->right, REGION_MCUBE_DEPTH - x, y, z, iter + 1);
    }
    else if (x < 0) {
        return getMCubeHelper(reg->left, x + REGION_MCUBE_DEPTH, y, z, iter + 1);
    }

    // Y
    if (y >= REGION_MCUBE_DEPTH) {
        return getMCubeHelper(reg->up, x, REGION_MCUBE_DEPTH - y, z, iter + 1);
    }
    else if (y < 0) {
        return getMCubeHelper(reg->down, x, y + REGION_MCUBE_DEPTH, z, iter + 1);
    }

    // Z
    if (z >= REGION_MCUBE_DEPTH) {
        return getMCubeHelper(reg->front, x, y, REGION_MCUBE_DEPTH - z, iter + 1);
    }
    else if (z < 0) {
        return getMCubeHelper(reg->back, x, y, z + REGION_MCUBE_DEPTH, iter + 1);
    }

    switch (reg->regType) {
        // EASY! Return the 1 value
        case FILLED:
            return *(reg->data);
        // Do typical indexing
        case MCUBED:
            return reg->data[x + z * REGION_MCUBE_DEPTH + y * REGION_MCUBE_DEPTH * REGION_MCUBE_DEPTH];
        // Since cubes are 2x bigger, this
        // will be a little more complex
        //
        // Divide the original indices by a conversion ratio which
        // if in region there are 16 cubes and 32 mini cubes then it would be 2
        //
        //          0
        // ---------
        // | 0 | 1 |  ...
        // |---+---|
        // |n+0|n+1|
        // ---------
        //
        case CUBED:
            const int CON_RATIO = REGION_MCUBE_DEPTH / REGION_CUBE_DEPTH;
            return reg->data[(x / CON_RATIO) + 
                             ((z / CON_RATIO) * REGION_CUBE_DEPTH) + 
                             ((y / CON_RATIO) * REGION_CUBE_DEPTH * REGION_CUBE_DEPTH)];
    }

    // This is never reached but its there in case it is
    return ERR_CUBE;
}
