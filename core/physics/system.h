#ifndef PHYSICS_SYS_H
#define PHYSICS_SYS_H

#include "component/physics_component.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <iostream>

#define PHYSICS_TPS 60

class PhysicsSnapshotBuffer {
public:
    std::vector<glm::vec3> positions;
    std::vector<uint8_t> alive;
    uint64_t tick{0};

    void resize(size_t n) {
        positions.resize(n, glm::vec3(0.0f));
        alive.resize(n, 0);
    }

    void clear() {
        std::fill(alive.begin(), alive.end(), 0);
    }
};

class PhysicsDoubleBuffer {
public:
    PhysicsSnapshotBuffer buffers[2];
    std::atomic<int> readIndex{1};

    PhysicsDoubleBuffer() {
        readIndex.store(1, std::memory_order_relaxed);
    }

    void publish(int writeIdx) {
        readIndex.store(writeIdx, std::memory_order_release);
    }
    const PhysicsSnapshotBuffer& getReadBuffer() const {
        return buffers[readIndex.load(std::memory_order_acquire)];
    }
    PhysicsSnapshotBuffer& getWriteBuffer(int writeIdx) {
        return buffers[writeIdx];
    }
};

class PhysicsSystem {
private:
    std::vector<PhysicsComponent*> bodies;
    std::vector<int> freeList;
    size_t aliveCount{0};

    PhysicsDoubleBuffer db;

    std::mutex mtx;
    std::thread physics_thd;
    std::atomic<bool> running;
    std::atomic<bool> paused;
public:
    PhysicsSystem() : running(false), paused(false) {}
    ~PhysicsSystem() { stop(); }
    
    int registerBody(PhysicsComponent* comp_ptr) {
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

    void unregisterBody(PhysicsComponent* comp_ptr) {
        std::lock_guard<std::mutex> lock(mtx);
        int slot = comp_ptr->slot;
        if (slot >= 0 && slot < static_cast<int>(bodies.size()) && bodies[slot] == comp_ptr) {
            bodies[slot] = nullptr;
            freeList.push_back(slot);
            comp_ptr->slot = -1;
            if (aliveCount > 0) aliveCount--;
        }
    }

    std::vector<PhysicsComponent*> getBodiesSnapshot() {
        std::lock_guard<std::mutex> lock(mtx);
        return bodies;
    }
    
    void start() {
        if (running.exchange(true)) return;
        physics_thd = std::thread(&PhysicsSystem::physicsLoop, this);
    }

    void stop() {
        if (!running.exchange(false)) return;
        if (physics_thd.joinable()) physics_thd.join();
    }

    template <class Fn>
    void forEachBody(Fn&& func) {
        std::vector<PhysicsComponent*> snapshot;
        {
            std::lock_guard<std::mutex> lock(mtx);
            snapshot.reserve(bodies.size());
            for (auto* b : bodies) if (b) snapshot.push_back(b);
        }
        for (auto* b : snapshot) func(*b);
    }

    const PhysicsDoubleBuffer& getDoubleBuffer() const { return db; }
    PhysicsDoubleBuffer& getDoubleBuffer() { return db; }

    void togglePause() { paused.store(!paused.load()); }
    void setPause(bool p) { paused.store(p); }
    bool isPaused() const { return paused.load(); }

    void stepOnce(f64 dT) {
        performStep(dT);
    }
private:
    void ensureBuffersCapacity(size_t sz) {
        for (i32 i{0}; i < 2; ++i) {
            if (db.buffers[i].positions.size() < sz) db.buffers[i].resize(sz);
        }
    }

    static glm::vec3 calculateAcceleration() {
        return glm::vec3(0.0, -4.5, 0.0);
    }

    static glm::vec3 calculateVelocity(glm::vec3& currentVelocity, glm::vec3& currentAcceleration, glm::vec3& acceleration, f32 dT) {
        glm::vec3 acc = currentAcceleration + acceleration;
        glm::vec3 result = currentVelocity + acc * (dT / 2);
        return result;
    }

    void performStep(f32 dT) {
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

    void physicsLoop() {
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
};

extern PhysicsSystem phySys;

#endif // PHYSICS_SYS_H