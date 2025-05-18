#include "shader.h"

#include <assert.h>

#include "glad/glad.h"

static bool checkCompileError(u32 shader);
static bool checkLinkError(u32 pId);

void compileShaderProg(const char* vertexSource, const char* fragmentSource, Shader* shader) {
    u32 vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    assert(checkCompileError(vertexShader));

    u32 fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    assert(checkCompileError(fragmentShader));

    shader->pId = glCreateProgram();
    glAttachShader(shader->pId, vertexShader);
    glAttachShader(shader->pId, fragmentShader);
    glLinkProgram(shader->pId);
    assert(checkLinkError(shader->pId));

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

static bool checkCompileError(u32 shader) {
    i32 success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader Compile Error, %s\n", infoLog);
        return false;
    }
    return true;
}

static bool checkLinkError(u32 pId) {
    i32 success;
    char infoLog[512];
    glGetShaderiv(pId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(pId, 512, NULL, infoLog);
        printf("ShaderProg Link Error, %s\n", infoLog);
        return false;
    }
    return true;
}