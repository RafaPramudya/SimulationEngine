#ifndef PHYSICS_UPDATER_H
#define PHYSICS_UPDATER_H

#include "component.h"
#include "physics_component.h"
#include "physics/system.h"

class Physics : public Component {
private:
    PhysicsComponent* physComp{nullptr};
    i32 slot{-1};

    f32 mass{1.0f};
    glm::vec3 initialVelocity;
public:
    Physics(f32 mass = 1.0f, const glm::vec3& initialVelocity = glm::vec3(0.0f)) 
    : mass(mass), initialVelocity(initialVelocity)
    {}

    void intialize() {
        auto& pc = entity->addComponent<PhysicsComponent>();
        pc.mass = mass;
        pc.initializeFromTransform(initialVelocity);
        slot = phySys.registerBody(&pc);
        physComp = &pc;
    }

    void update() override {
        if(!physComp) {
            intialize();
        }
        const auto& buf = phySys.getDoubleBuffer().getReadBuffer();
        if(slot >= 0 && slot < static_cast<int>(buf.positions.size()) && buf.alive[slot]) {
            entity->getComponent<Transform>().setTranslation(buf.positions[slot]);
        }
    }

    void onRemoved() {
        if(physComp) {
            phySys.unregisterBody(physComp);
            physComp = nullptr;
            slot = -1;
        }
    }
};

#endif //PHYSICS_UPDATER_H