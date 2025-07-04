#include "light.h"

#include "transform.h"

#include <exception>
#include <string>

std::vector<Light*> Light::lights;

void Light::setLight(f32 constant, f32 linear, f32 quadratic, glm::vec3& color) {
    attenuation.constant = constant;
    attenuation.linear = linear;
    attenuation.quadratic = quadratic;

    Light::color = color;
}

void Light::setLight(f32 constant, f32 linear, f32 quadratic, glm::vec3&& color) {
    attenuation.constant = constant;
    attenuation.linear = linear;
    attenuation.quadratic = quadratic;

    Light::color = color;
}

void Light::setUniform(ShaderProg& shader) {
    if (lights.size() > MAX_LIGHT) throw std::exception("Jumlah light melebihi batas");
    for (i32 i{0}; i < lights.size(); i++) {
        if (!lights[i]->entity->hasComponent<Transform>()) throw std::exception("Light component tidak ada Transform");
        auto& transform = lights[i]->entity->getComponent<Transform>();
        std::string lightName = "lights[" + std::to_string(i) + "]";

        shader.setUniform((lightName + ".position").c_str(), transform.getTranslation());
        shader.setUniform((lightName + ".color").c_str(), lights[i]->getColor());
        shader.setUniform((lightName + ".constant").c_str(), lights[i]->getAttenuation().constant);
        shader.setUniform((lightName + ".linear").c_str(), lights[i]->getAttenuation().linear);
        shader.setUniform((lightName + ".quadratic").c_str(), lights[i]->getAttenuation().quadratic);
    }

        std::string lightName = "lights[" + std::to_string(lights.size()) + "]";
        static auto emptyPos = glm::vec3(0.0f);
        static auto emptyCol = glm::vec3(0.0f);

        shader.setUniform((lightName + ".position").c_str(), emptyPos);
        shader.setUniform((lightName + ".color").c_str(), emptyCol);
        shader.setUniform((lightName + ".constant").c_str(), 0.0f);
        shader.setUniform((lightName + ".linear").c_str(), 0.0f);
        shader.setUniform((lightName + ".quadratic").c_str(), 0.0f);
}