#ifndef LIGHT_H
#define LIGHT_H

#include "utils/types.h"
#include "model.h"

#include <glm/glm.hpp>

typedef struct Attenuation {
    f32 constant;
    f32 linear;
    f32 quadratic;
} Attenuation;

class Light : public Model
{
private:
    Attenuation attenuation;
    glm::vec3 color;
    const char* name;
public:
    Light(const char* name, f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz) 
        : Model(lpverts, vertsSz, lpinds, indsSz) { Light::name = name; }
    ~Light() = default;

    inline const char* getName() const {return name;}
    glm::vec3& getColor() {return color;}
    Attenuation& getAttenuation() { return attenuation; } 

    void setLight(f32 constant, f32 linear, f32 quadratic, glm::vec3& color);
    void setLight(f32 constant, f32 linear, f32 quadratic, glm::vec3&& color);
};


#endif // LIGHT_H