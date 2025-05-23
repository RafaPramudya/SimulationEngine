#ifndef RENDERER_H
#define RENDERER_H

#include "utils/types.h"
#include "shader.h"
#include "texture.h"

typedef struct RenderState_s {
    u32 VAO;
    u32 VBO;
    u32 EBO;

    Shader main_shader;

    Texture ttsTexture;
} RenderState;

void PrepareRenderer(void);
void RendererResize(u32 width, u32 height);
void RenderLoop(void);
void QuitRenderer(void);

#endif