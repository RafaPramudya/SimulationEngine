#include "shader.h"

#include <cassert>
#include <cstdio>
#include <string>

#include "glad/glad.h"
#include "utils/importer.h"

#include <glm/gtc/type_ptr.hpp>

static bool checkCompileError(u32 shader);
static bool checkLinkError(u32 pId);

Shader::Shader(GLenum type, const char* filename) {
    char* source = getContentFromFile(filename);

    Shader::id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    freeContent(source);
    assert(checkCompileError(id));
}

Shader::~Shader() {
    glDeleteShader(id);
}

ShaderProg::ShaderProg() {
    pId = glCreateProgram();
}
ShaderProg::~ShaderProg() {
    glDeleteProgram(pId);
}
void ShaderProg::use() {
    glUseProgram(pId);
}

void ShaderProg::setUniform(const char* location, i32 value) {
    glUniform1i(glGetUniformLocation(pId, location), value);
}

void ShaderProg::setUniform(const char* location, f32 value) {
    glUniform1f(glGetUniformLocation(pId, location), value);
}

void ShaderProg::setUniform(const char* location, glm::mat4 matrix) {
    glUniformMatrix4fv(glGetUniformLocation(pId, location), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProg::setUniform(const char* location, glm::vec3& vector) {
    glUniform3fv(glGetUniformLocation(pId, location), 1, glm::value_ptr(vector));
}



void ShaderProg::attachShader(Shader& shader) {
    glAttachShader(pId, shader.id);
}
void ShaderProg::linkProgram() {
    glLinkProgram(pId);
    assert(checkLinkError(pId));
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