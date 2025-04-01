#include <glad/glad.h>

#ifndef SHADER_H
#define SHADER_H

char* getFileContent(const char* fileName);

void compileShader(GLuint* shaderID, GLenum shaderType, const char* shaderFilePath);

GLuint linkShader(GLuint vertexShaderID, GLuint fragmentShaderID);

#endif
