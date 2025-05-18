#ifndef RENDERER_H
#define RENDERER_H

#include "utils/types.h"
#include "shader.h"

typedef struct RenderState_s {
    u32 VAO;
    u32 VBO;

    Shader main_shader;
} RenderState;

void PrepareRenderer(void);
void RendererResize(u32 width, u32 height);
void RenderLoop(void);

#endif