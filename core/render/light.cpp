#include "light.h"

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