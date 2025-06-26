#ifndef RENDERER_H
#define RENDERER_H

#include "utils/types.h"
#include "shader.h"
#include "texture.h"
#include "object.h"
#include "model.h"
#include "light.h"

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

    // Object Section
    std::optional<Model> basic;
    std::optional<Light> light;
};

extern Renderer* renderer;

#endif