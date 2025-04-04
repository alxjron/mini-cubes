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

    // Temp stuff ------------------------>
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    GLuint VBO, VAO; 

    // set up vertex buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set up vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    // Declare transform matrices
    mat4s model = glms_mat4_identity();
    glm_translate(model.raw, (vec3s) {.x = 0.0f, .y = 0.0f, .z = -3.0f}.raw);
    mat4s view = glms_mat4_zero();
    mat4s projection = glms_mat4_zero();

    int exited = 0;

    while (!exited) {

        glViewport(0, 0, 800, 800);
    
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exited = 1;
            }
        }

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

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}

