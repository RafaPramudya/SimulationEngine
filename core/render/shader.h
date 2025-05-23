#ifndef SHADER_H
#define SHADER_H

#include "utils/types.h"

typedef struct Shader_s {
    u32 pId;
} Shader;

typedef unsigned int GLenum;

u32 compileShader(GLenum type, const char* source);
void compileProgram(const char* vertexFilename, const char* fragmentFilename, Shader* shader);

void freeShader(Shader shader);

#endif // SHADER_H