/**
 * The main file of the game
 *
 */
#include <stdio.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <cglm/struct.h>
#include <cglm/util.h>

#include "shader.h"
#include "camera.h"
#include "mesh.h"

int main() {
    printf("Hello world!\n");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR: SDL could not be initialized\n");
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
            "Mini Cubes",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800, 800,
            SDL_WINDOW_OPENGL
            );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    //SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_CreateContext(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    GLuint verShaderID, fragShaderID;

    compileShader(&verShaderID, GL_VERTEX_SHADER, "assets/vertex.glsl");
    compileShader(&fragShaderID, GL_FRAGMENT_SHADER, "assets/frag.glsl");

    GLuint programID = linkShader(verShaderID, fragShaderID);

    Camera* cam = initCamera();

    // Test mesh
    Mesh* test = initMesh();

    Vertex verts[] = {
        /*
         * 0 -------- 1
         *   |      |
         *   |      |
         *   |      |
         * 2 -------- 3
         *
         * 4 -------- 5
         *   |      |
         *   |      |
         *   |      |
         * 6 -------- 7
         *
         *
         */
        (Vertex) {.pos = {-0.5f, 0.5f, 0.5f}},
        (Vertex) {.pos = {0.5f, 0.5f, 0.5f}},
        (Vertex) {.pos = {-0.5f, -0.5f, 0.5f}},
        (Vertex) {.pos = {0.5f, -0.5f, 0.5f}},

        (Vertex) {.pos = {-0.5f, 0.5f, -0.5f}},
        (Vertex) {.pos = {0.5f, 0.5f, -0.5f}},
        (Vertex) {.pos = {-0.5f, -0.5f, -0.5f}},
        (Vertex) {.pos = {0.5f, -0.5f, -0.5f}},
    };

    GLuint elems[] = {
        // Front
        0, 1, 3,
        0, 3, 2,

        // Left
        4, 0, 2,
        4, 2, 6,

        // Right
        1, 5, 7,
        1, 7, 3,

        // Back
        7, 5, 4, 
        7, 4, 6, 

        // Top
        0, 4, 5,
        0, 5, 1,

        // Bottom
        2, 3, 7,
        2, 7, 6,
    };

    setMeshVertPointer(test, verts, 8);
    setMeshElemPointer(test, elems, 36);

    // Declare transform matrices
    mat4s model = glms_mat4_identity();
    glm_translate(model.raw, (vec3s) {.x = 0.0f, .y = 0.0f, .z = -2.0f}.raw);
    mat4s view = glms_mat4_zero();
    mat4s projection = glms_mat4_zero();

    SDL_SetRelativeMouseMode(SDL_TRUE);
    Uint32 lastUpdate = SDL_GetTicks();

    int exited = 0;

    while (!exited) {
        Uint32 current = SDL_GetTicks();
        float deltaTime = (current - lastUpdate) / 1000.0f;

        glViewport(0, 0, 800, 800);
    
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exited = 1;
            }
            updateCameraLook(cam, event, deltaTime);
        }

        updateCameraMovement(cam, deltaTime);

        glClearColor(0.3f, 0.3f, 0.6f, 1.f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // Set transform matrices
        getView(cam, view.raw);
        glm_perspective(glm_rad(60.0f), 1.0f, 0.1f, 100.0f, projection.raw);

        glUseProgram(programID);

        GLuint modelLoc = glGetUniformLocation(programID, "model");
        GLuint viewLoc = glGetUniformLocation(programID, "view");
        GLuint projectionLoc = glGetUniformLocation(programID, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model.raw[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view.raw[0][0]);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection.raw[0][0]);

        // Draw meshes
        drawMesh(*test);

        SDL_GL_SwapWindow(window);
        lastUpdate = current;
    }

    return 0;
}

