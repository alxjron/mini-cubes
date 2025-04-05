#include "camera.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <cglm/cam.h>
#include <cglm/struct.h>
#include <cglm/util.h>
#include <cglm/struct.h>

/**
 * Helper function for setting up the front
 * vector of the camera.
 *
 */
void setFront(Camera* camPtr) {
    float pitch = camPtr->pitch;
    float yaw = camPtr->yaw;

    camPtr->front.x = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    camPtr->front.y = -sin(glm_rad(pitch));
    camPtr->front.z = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    camPtr->front = glms_normalize(camPtr->front);
}

/**
 * Initalizes the camera structure.
 *
 */
Camera* initCamera() {
    Camera* result = malloc(sizeof(Camera));

    if (result == NULL) {
        fprintf(stderr, "ERROR: unable to allocate memory for program!\n");
        exit(1);
    }

    result->yaw = -90.0f;
    result->pitch = 0.0f;
    result->position = (vec3s) {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    result->up = (vec3s) {.x = 0.0f, .y = 1.0f, .z = 0.0f};
    result->front = (vec3s) {.x = 0.0f, .y = 0.0f, .z = 0.0f};

    setFront(result);

    return result;
}

/**
 * Gets the view matrix of the camera
 *
 */
void getView(Camera* camPtr, mat4 view) {
    setFront(camPtr);

    vec3s dir = {.x = camPtr->position.x + camPtr->front.x,
                 .y = camPtr->position.y + camPtr->front.y,
                 .z = camPtr->position.z + camPtr->front.z};

    glm_look(camPtr->front.raw, dir.raw, camPtr->up.raw, view);
}
