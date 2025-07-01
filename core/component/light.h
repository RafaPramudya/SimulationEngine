#ifndef LIGHT_H
#define LIGHT_H

#include "component.h"
#include "render/shader.h"

#include "utils/types.h"
#include <glm/glm.hpp>

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
    const char* name;

public:
    static std::vector<Light*> lights;

    Light(const char* name) : name(name) { lights.push_back(this); }
    ~Light() = default;

    inline const char* getName() const {return name;}
    glm::vec3& getColor() {return color;}
    Attenuation& getAttenuation() { return attenuation; } 

    void setLight(f32 constant, f32 linear, f32 quadratic, glm::vec3& color);
    void setLight(f32 constant, f32 linear, f32 quadratic, glm::vec3&& color);
    void setUniform(ShaderProg& shader);
};


#endif // LIGHT_H