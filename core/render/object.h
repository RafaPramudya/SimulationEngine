#ifndef OBJECT_H
#define OBJECT_H

#include "utils/types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Object {
public:

    Object() = default;
    Object(f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz);
    ~Object();

    void addAttribute(size_t size);
    void compileAttribute();

    inline u32 getVAO() const { return VAO; }
    inline u32 getVBO() const { return VBO; }
    inline u32 getEBO() const { return EBO; }
private:
    u32 VAO;
    u32 VBO;
    u32 EBO;
};

#endif