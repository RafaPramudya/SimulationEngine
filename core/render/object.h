#ifndef OBJECT_H
#define OBJECT_H

#include "utils/types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scalar;

    Transform()
        : translation(0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scalar(1.0f)
    {}
    ~Transform() = default;

    void translate(const glm::vec3& delta) {
        translation += delta;
    }

    void rotate(float deg, const glm::vec3& axis) {
        glm::quat delta = glm::angleAxis(glm::radians(deg), glm::normalize(axis));
        rotation = delta * rotation;
    }

    void scale(const glm::vec3& factor) {
        scalar *= factor;
    }

    glm::mat4 getMatrix() {
        glm::mat4 model(1.0f);
        model = glm::translate(model, translation);
        model *= glm::mat4_cast(rotation);
        model = glm::scale(model, scalar);
        return model;
    }
};

class Object {
public:
    Transform transform;

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