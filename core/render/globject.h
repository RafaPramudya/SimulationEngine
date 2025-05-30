#ifndef GLOBJECT_H
#define GLOBJECT_H

#include "utils/types.h"

typedef struct GLObj
{
    u32 VAO;
    u32 VBO;
    u32 EBO;
} GLObj;

void createGLObject(GLObj* object, f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz);
void startsVertsAttribs(void);
void addVertsAttrib(size_t size);
void compileVertsAttrib(void);

#endif