#include "camera.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <cglm/cam.h>
#include <cglm/struct.h>
#include <cglm/util.h>
#include <cglm/struct.h>
#include <cglm/vec3.h>

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
    glm_normalize(camPtr->front.raw);
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

    glm_lookat(camPtr->position.raw, dir.raw, camPtr->up.raw, view);
}

/**
 * Updates the camera's rotation
 * based on player mouse events.
 *
 */
void updateCameraLook(Camera *camPtr, SDL_Event event, float deltaTime) {
    float sensitivity = 5.0f;

    if (event.type == SDL_MOUSEMOTION) {
        camPtr->yaw += event.motion.xrel * sensitivity * deltaTime;
        camPtr->pitch += event.motion.yrel * sensitivity * deltaTime;

        camPtr->pitch = glm_clamp(camPtr->pitch, -89.0f, 89.0f);
    }
}

/**
 * Updates the camera's position 
 * based on keyboard input.
 *
 */
void updateCameraMovement(Camera *camPtr, float deltaTime) {
    float speed = 5.0f;
    vec3s direction = {.x = 0.0f, .y = 0.0f, .z = 0.0f};

    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_W]) 
        direction.z = 1.0f;
    else if (keyState[SDL_SCANCODE_S])
        direction.z = -1.0f;

    if (keyState[SDL_SCANCODE_D]) 
        direction.x = 1.0f;
    else if (keyState[SDL_SCANCODE_A])
        direction.x = -1.0f;

    if (keyState[SDL_SCANCODE_SPACE])
        direction.y = 1.0f;

    else if (keyState[SDL_SCANCODE_LSHIFT])
        direction.y = -1.0f;

    float frontMove = speed * deltaTime * direction.z;
    float sideMove = speed * deltaTime * direction.x;
    float yMove = speed * deltaTime * direction.y;

    glm_vec3_add(glms_vec3_mul(glms_vec3_normalize(glms_vec3_cross(camPtr->front, camPtr->up)), (vec3s) {.x = sideMove, .y = 0.0f, .z = sideMove}).raw, camPtr->position.raw, camPtr->position.raw);
    glm_vec3_add(glms_vec3_mul(camPtr->front, (vec3s) {.x = frontMove, .y = 0.0f, .z = frontMove}).raw, camPtr->position.raw, camPtr->position.raw);
    glm_vec3_add(camPtr->position.raw, (vec3s) {.x = 0.0f, .y = yMove, .z = 0.0f}.raw, camPtr->position.raw);
}
