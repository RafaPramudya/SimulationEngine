#ifndef RENDERER_H
#define RENDERER_H

#include "utils/types.h"
#include "shader.h"
#include "texture.h"
#include "globject.h"

#include <optional>

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void renderEvent();
    void render();

    void resize(u32 width, u32 height);
private:
    ShaderProg main_prog;
    ShaderProg light_prog;

    // Texture
    std::optional<Texture> tts;

    // Object Section
    std::optional<Object> basic;
    std::optional<Object> light;
};

extern Renderer* renderer;

#endif