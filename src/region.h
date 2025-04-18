#include "mesh.h"
#include <cglm/struct.h>

#ifndef REGION_H
#define REGION_H

extern const int REGION_CUBE_DEPTH;
extern const int REGION_MCUBE_DEPTH;

extern char* ERR_CUBE;
extern char* AIR_CUBE;

enum RegionType {
    FILLED,
    CUBED,
    MCUBED
};

typedef struct _region {
    Mesh* meshPtr;

    enum RegionType regType;
    char** data;

    struct _region* up;
    struct _region* down;
    struct _region* left;
    struct _region* right;
    struct _region* front;
    struct _region* back;
} Region;

Region* initRegion(vec3s pos);

/**
 * Simply connects the two regions to each other.
 *
 * Returns 1 if success, 0 otherwise.
 */
int connectRegions(Region* src, Region* dest, enum CubeFace face);

/**
 * Finds and detaches a neighboring region.
 *
 * Returns 1 if success, 0 otherwise.
 */
int detachRegions(Region* src, Region* reg);

/**
 * Functions for setting the region
 * data.
 *
 */
int fillRegion(char* cubeID, Region* regPtr);
int setMCube(char* cubeID, Region* regPtr, vec3s pos);
int setCube(char* cubeID, Region* regPtr, vec3s pos);

/**
 * Functions for getting cube data from
 * region.
 *
 * Returns "ERROR" when it fails
 *
 */
char* getMCube(Region* reg, int x, int y, int z);

/**
 * Modifies the mesh to fit with the
 * data.
 *
 */
void updateRegionMesh(Region* reg);

void freeRegion(Region** regPptr);

#endif
