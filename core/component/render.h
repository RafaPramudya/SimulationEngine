#ifndef RENDER_H
#define RENDER_H

#include "component.h"
#include "transform.h"
#include "light.h"
#include "render/model.h"
#include "render/camera.h"

#include <exception>

class Render : public Component {
private:
    ShaderProg* shader;
public:
    Model model;
    Render(std::string path, ShaderProg* shader) : model(path), shader(shader) {}
    Render(f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz, ShaderProg* shader) : 
        model(lpverts, vertsSz, lpinds, indsSz), shader(shader) {}
    ~Render() = default;

    inline void changeShaderProgram(ShaderProg* shader) { Render::shader = shader; }

    void render() override {
        shader->use();
        if (!entity->hasComponent<Transform>()) std::exception("Transform tidak dideklarasi");
        auto& transform = entity->getComponent<Transform>();

        shader->setUniform("model", transform.getMatrix());
        shader->setUniform("view", camera->getView());
        shader->setUniform("projection", camera->getProjection());
        if (entity->hasComponent<Light>()) {
            shader->setUniform("color", entity->getComponent<Light>().getColor());
        } else 
            Light::setUniform(*shader);
        model.draw(*shader);
    }
};

#endif