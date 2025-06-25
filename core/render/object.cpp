#include "object.h"

#include "glad/glad.h"

#include <string.h>

typedef struct VertAttrib {
    size_t nrComponents;
    size_t offset;
} VertAttrib;

static size_t vertsAttribsStride = 0;
static size_t vertsAttribsIndex = 0;
static VertAttrib vertexAttribs[16];

Object::Object(f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertsSz, (const void*)lpverts, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indsSz, (const void*)lpinds, GL_STATIC_DRAW);

    memset(vertexAttribs, 0, sizeof(vertexAttribs));
    vertsAttribsIndex = 0;
    vertsAttribsStride = 0;
}

Object::~Object() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Object::addAttribute(size_t size) {
    size_t index = vertsAttribsIndex++;
    vertexAttribs[index].nrComponents = size;
    vertexAttribs[index].offset = vertsAttribsStride;
    vertsAttribsStride += size * sizeof(float);
}

void Object::compileAttribute() {
    for (size_t i = 0; i < vertsAttribsIndex; ++i) {
        glVertexAttribPointer(i, vertexAttribs[i].nrComponents, GL_FLOAT, GL_FALSE, vertsAttribsStride, (const void*)vertexAttribs[i].offset);
        glEnableVertexAttribArray(i);
    }
}
