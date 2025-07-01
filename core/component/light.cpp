#include "light.h"

#include "transform.h"
#include <exception>

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
    std::string lightName = name;
    if (!entity->hasComponent<Transform>()) throw std::exception("Light component tidak ada Transform");
    auto& transform = entity->getComponent<Transform>();
    
    shader.setUniform((lightName + ".position").c_str(), transform.getTranslation());
    shader.setUniform((lightName + ".color").c_str(), color);
    shader.setUniform((lightName + ".constant").c_str(), attenuation.constant);
    shader.setUniform((lightName + ".linear").c_str(), attenuation.linear);
    shader.setUniform((lightName + ".quadratic").c_str(), attenuation.quadratic);
}