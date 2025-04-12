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
    char** newData = malloc(strlen(cubeID) + 1);
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

void updateRegionMesh(Region* reg) {
    if (reg == NULL) 
        return;

    clearMeshVertPointer(reg->meshPtr);
    clearMeshElemPointer(reg->meshPtr);

    int x, y, z;

    /**
     * Note: y starts at the top of a region
     * and goes down
     *
     */

    for (y = 0; y < REGION_MCUBE_DEPTH; y++) {
        for (x = 0; x < REGION_MCUBE_DEPTH; x++) {
            for (z = 0; z < REGION_MCUBE_DEPTH; z++) {
                float rx, ry, rz;
                rx = 0.25 * (float) x;
                ry = -0.25 * (float) y;
                rz = 0.25 * (float) z;
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

    // Yes Y is flipped because I decided that
    // y = 0 starts at the very top
    if (y > 0) {
        return getMCubeHelper(reg->up, x, y - REGION_MCUBE_DEPTH, z, iter + 1);
    }
    else if (y <= -REGION_MCUBE_DEPTH) {
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
