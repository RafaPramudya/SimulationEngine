#ifndef RENDERER_H
#define RENDERER_H

#include "utils/types.h"
#include "shader.h"
#include "component/component.h"

#include <optional>

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void renderEventUpdate();
    void render();

    void resize(u32 width, u32 height);
private:
    ShaderProg main_prog;
    ShaderProg light_prog;
};

extern Renderer* renderer;

#endif