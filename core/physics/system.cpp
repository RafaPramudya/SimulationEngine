#include "system.h"

static glm::vec3 calculateAcceleration() {
    return glm::vec3(0.0, -4.5, 0.0);
}

static glm::vec3 calculateVelocity(glm::vec3& currentVelocity, glm::vec3& currentAcceleration, glm::vec3& acceleration, f32 dT) {
    glm::vec3 acc = currentAcceleration + acceleration;
    glm::vec3 result = currentVelocity + acc * (dT / 2);
    return result;
}

int PhysicsSystem::registerBody(PhysicsComponent* comp_ptr) {
    std::lock_guard<std::mutex> lock(mtx);
        i32 slot;
        if (!freeList.empty()) {
            slot = freeList.back();
            freeList.pop_back();
            bodies[slot] = comp_ptr;
            aliveCount++;
        } else {
            slot = static_cast<int>(bodies.size());
            bodies.push_back(comp_ptr);
        }
        comp_ptr->slot = slot;
        ensureBuffersCapacity(bodies.size());
        return slot;
}
void PhysicsSystem::unregisterBody(PhysicsComponent* comp_ptr) {
    std::lock_guard<std::mutex> lock(mtx);
    int slot = comp_ptr->slot;
    if (slot >= 0 && slot < static_cast<int>(bodies.size()) && bodies[slot] == comp_ptr) {
        bodies[slot] = nullptr;
        freeList.push_back(slot);
        comp_ptr->slot = -1;
        if (aliveCount > 0) aliveCount--;
    }
}

std::vector<PhysicsComponent*> PhysicsSystem::getBodiesSnapshot() {
    std::lock_guard<std::mutex> lock(mtx);
    return bodies;
}

void PhysicsSystem::start() {
    if (running.exchange(true)) return;
    physics_thd = std::thread(&PhysicsSystem::physicsLoop, this);
}
void PhysicsSystem::stop() {
    if(!running.exchange(false)) return;
    if (physics_thd.joinable()) physics_thd.join();
}

template<class Fn>
void PhysicsSystem::forEachBody(Fn&& func) {
    std::vector<PhysicsComponent*> snapshot;
    {
        std::lock_guard<std::mutex> lock(mtx);
        snapshot.reserve(bodies.size());
        for (auto* b : bodies) if (b) snapshot.push_back(b);
    }
    for (auto* b : snapshot) func(*b);
}

void PhysicsSystem::ensureBuffersCapacity(size_t sz) {
    for (i32 i{0}; i < 2; ++i) {
        if (db.buffers[i].positions.size() < sz) db.buffers[i].resize(sz);
    }
}

void PhysicsSystem::performStep(f32 dT) {
    std::vector<PhysicsComponent*> snapshot;
    {
        // Copy bodies to snapshot
        std::lock_guard<std::mutex> lock(mtx);
        snapshot = bodies;
    }

    for (auto* b : snapshot) {
        if (!b) continue;

        glm::vec3 pos = b->position;
        glm::vec3 prevPos = b->prevPosition;
        glm::vec3 velocity = b->velocity;
        glm::vec3 acceleration = b->acceleration;

        b->acceleration = glm::vec3(0.0f);
        b->acceleration += calculateAcceleration();

        b->velocity = calculateVelocity(velocity, acceleration, b->acceleration, dT);

        b->position = pos + velocity * (f32)dT + acceleration * ((f32)(dT * dT) / 2);
        b->prevPosition = pos;
    }
}

void PhysicsSystem::physicsLoop() {
    using clock = std::chrono::high_resolution_clock;
    const auto dT = 1.0 / (f64)PHYSICS_TPS;
    const auto interval = std::chrono::nanoseconds(static_cast<long long>(dT * 1e9));
    i32 writeIdx = 0;
    uint64_t tick = 0;

    auto last = clock::now();
    auto acc = 0.0;

    while (running.load(std::memory_order_relaxed)) {
        auto now = clock::now();
        std::chrono::duration<double> frame_time = now - last;
        last = now;

        if (paused.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        } else {
            acc += frame_time.count();
        }

        while (acc >= dT) {
            // Perform step
            performStep(dT);

            auto& buf = db.getWriteBuffer(writeIdx);
            {
                // Copy bodies to snapshot
                std::lock_guard<std::mutex> lock(mtx);
                buf.resize(bodies.size());

                for (size_t i{0}; i < bodies.size(); ++i) {
                    auto* b = bodies[i];
                    if (b) {
                        buf.positions[i] = b->position;
                        buf.alive[i] = 1;
                    } else {
                        buf.positions[i] = glm::vec3(0.0f);
                        buf.alive[i] = 0;
                    }
                }
            }
            buf.tick = ++tick;

            db.publish(writeIdx);
            writeIdx ^= 1;

            acc -= dT;
        }

        // Avoid busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  
    }
}

PhysicsSystem phySys;