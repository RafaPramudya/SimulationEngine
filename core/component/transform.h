#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform : public Component {
private:
    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scalar;
public:
    Transform()
        : translation(0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scalar(1.0f)
    {}
    ~Transform() = default;

    auto& getTranslation() {
        return translation;
    }
    auto& getRotation() {
        return rotation;
    }
    auto& getScalar() {
        return scalar;
    }

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

#endif