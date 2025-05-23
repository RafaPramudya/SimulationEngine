#include "shader.h"

#include <assert.h>

#include "glad/glad.h"
#include "utils/importer.h"

static bool checkCompileError(u32 shader);
static bool checkLinkError(u32 pId);

u32 compileShader(GLenum type, const char* source) {
    u32 shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    assert(checkCompileError(shader));

    return shader;
}

void compileProgram(const char* vertexFilename, const char* fragmentFilename, Shader* shader) {
    char* vertexSource = getContentFromFile(vertexFilename);
    char* fragmentSource = getContentFromFile(fragmentFilename);

    u32 vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    u32 fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    shader->pId = glCreateProgram();
    glAttachShader(shader->pId, vertexShader);
    glAttachShader(shader->pId, fragmentShader);
    glLinkProgram(shader->pId);
    assert(checkLinkError(shader->pId));

    freeContent(vertexSource);
    freeContent(fragmentSource);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void freeShader(Shader shader) {
    glDeleteProgram(shader.pId);
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