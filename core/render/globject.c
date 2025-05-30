#include "globject.h"

#include "glad/glad.h"

#include <string.h>

typedef struct VertAttrib {
    size_t nrComponents;
    size_t offset;
} VertAttrib;

static size_t vertsAttribsStride = 0;
static size_t vertsAttribsIndex = 0;
static VertAttrib vertexAttribs[16];

void createGLObject(GLObj* object, f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz) {
    glGenVertexArrays(1, &object->VAO);
    glBindVertexArray(object->VAO);

    glGenBuffers(1, &object->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, object->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertsSz, (const void*)lpverts, GL_STATIC_DRAW);

    glGenBuffers(1, &object->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indsSz, (const void*)lpinds, GL_STATIC_DRAW);
}

void startsVertsAttribs(void) {
    memset(vertexAttribs, 0, sizeof(vertexAttribs));
    vertsAttribsIndex = 0;
    vertsAttribsStride = 0;
}
void addVertsAttrib(size_t size) {
    size_t index = vertsAttribsIndex++;
    vertexAttribs[index].nrComponents = size;
    vertexAttribs[index].offset = vertsAttribsStride;
    vertsAttribsStride += size * sizeof(float);
}
void compileVertsAttrib(void) {
    for (size_t i = 0; i < vertsAttribsIndex; ++i) {
        glVertexAttribPointer(i, vertexAttribs[i].nrComponents, GL_FLOAT, GL_FALSE, vertsAttribsStride, (const void*)vertexAttribs[i].offset);
        glEnableVertexAttribArray(i);
    }
}