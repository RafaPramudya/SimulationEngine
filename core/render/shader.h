#ifndef SHADER_H
#define SHADER_H

#include "utils/types.h"

typedef struct Shader_s {
    u32 pId;
} Shader;

void compileShaderProg(const char* vertexSource, const char* fragmentSource, Shader* shader);

#endif // SHADER_H