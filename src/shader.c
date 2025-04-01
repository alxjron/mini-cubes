/**
 * This file holds defitions for compiling and linking shader files
 *
 */
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Takes in a file name and returns a pointer to the first character
 * at the start of the file.
 *
 */
char* getFileContent(const char* fileName) {
    FILE* fp;
    long size = 0;
    char* content;

    // Read file to get the size
    fp = fopen(fileName, "rb");
    if (fp == NULL) 
        return "";

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp) + 1;
    fclose(fp);

    // Read to return content
    fp = fopen(fileName, "r");
    content = memset(malloc(size), '\0', size);
    fread(content, 1, size - 1, fp);
    fclose(fp);

    return content;
}

/**
 * Compiles a shader of any type and stores the ID in a given pointer.
 *
 */
void compileShader(GLuint* shaderID, GLenum shaderType, const char* shaderFilePath) {
    GLint isCompiled = 0;
    GLint maxLength = 0;
    char* infoLog = malloc(1024);

    const char* shaderSource = getFileContent(shaderFilePath);

    *shaderID = glCreateShader(shaderType);
    if (*shaderID == 0)
        fprintf(stderr, "ERROR: Could not load shader %s\n", shaderFilePath);

    glShaderSource(*shaderID, 1, (const char**) &shaderSource, NULL);
    glCompileShader(*shaderID);
    glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        fprintf(stderr, "ERROR: Compiling shader failed %s\n", shaderFilePath);

        glGetShaderiv(*shaderID, GL_INFO_LOG_LENGTH, &maxLength);
        glGetShaderInfoLog(*shaderID, maxLength, &maxLength, infoLog);

        fprintf(stderr, "%s\n", infoLog);

        glDeleteShader(*shaderID);
    }

    free(infoLog);
}

/**
 * Links fragment and vertex shader together. 
 * Returns the program ID.
 *
 */
GLuint linkShader(GLuint vertexShaderID, GLuint fragmentShaderID) {
    GLuint programID = 0;
    GLint isLinked = 0;
    GLint maxLength = 0;
    char* infoLog = malloc(1024);

    programID = glCreateProgram();

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);

    if (isLinked == GL_FALSE) {
        fprintf(stderr, "ERROR: Linking shader failed\n");

        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
        glGetProgramInfoLog(programID, maxLength, &maxLength, infoLog);

        fprintf(stderr, "%s\n", infoLog);

        glDeleteProgram(programID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        free(infoLog);
        return 0;
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    free(infoLog);

    return programID;
}
