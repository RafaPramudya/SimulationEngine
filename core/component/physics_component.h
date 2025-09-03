#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "component.h"
#include "transform.h"
#include "utils/types.h"

#include <glm/glm.hpp>
#include <atomic>
#include <exception>

class PhysicsComponent : public Component {
public:
    glm::vec3 position{0.0f};
    glm::vec3 prevPosition{0.0f};
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration = glm::vec3(0.0f);
    f32 mass{1.0f};

    int32_t slot{-1};

    PhysicsComponent() = default;

    void initializeFromTransform(const glm::vec3& initialVelocity = glm::vec3(0.0), f32 tps = 60.0f) {
        if (!entity->hasComponent<Transform>()) {
            throw std::runtime_error("PhysicsComponent requires Transform component on the same entity");
        }

        position = entity->getComponent<Transform>().getTranslation();
        f32 dT = 1.0f / tps;
        prevPosition = position - initialVelocity * dT;
        velocity = initialVelocity;
        acceleration = glm::vec3(0.0);
    }

    void applyForce(const glm::vec3& force) {
        acceleration += force / mass;
    }
};

#endif //PHYSICS_COMPONENT_H