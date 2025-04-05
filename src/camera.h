#include <SDL2/SDL_events.h>
#include <cglm/struct.h>
#include <cglm/mat4.h>

#ifndef CAMERA_H
#define CAMERA_H

typedef struct _camera {
    float yaw;
    float pitch;
    vec3s position;
    vec3s front;
    vec3s up;
} Camera;

Camera* initCamera();

void getView(Camera* camPtr, mat4 view);

void updateCamera(Camera* camPtr, SDL_Event event, float deltaTime);

#endif
