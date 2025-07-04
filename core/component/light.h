#ifndef LIGHT_H
#define LIGHT_H

#include "component.h"
#include "render/shader.h"

#include "utils/types.h"
#include <glm/glm.hpp>

#define MAX_LIGHT 32

typedef struct Attenuation {
    f32 constant;
    f32 linear;
    f32 quadratic;
} Attenuation;

class Light : public Component
{
private:
    Attenuation attenuation;
    glm::vec3 color;

public:
    static std::vector<Light*> lights;

    Light() { lights.push_back(this); }
    ~Light() = default;

    glm::vec3& getColor() {return color;}
    Attenuation& getAttenuation() { return attenuation; } 

    void setLight(f32 constant, f32 linear, f32 quadratic, glm::vec3& color);
    void setLight(f32 constant, f32 linear, f32 quadratic, glm::vec3&& color);
    static void setUniform(ShaderProg& shader);
};


#endif // LIGHT_H