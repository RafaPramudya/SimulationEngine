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
    
    int registerBody(PhysicsComponent* comp_ptr);
    void unregisterBody(PhysicsComponent* comp_ptr);

    std::vector<PhysicsComponent*> getBodiesSnapshot();
    
    void start();
    void stop();

    template <class Fn>
    void forEachBody(Fn&& func);

    const PhysicsDoubleBuffer& getDoubleBuffer() const { return db; }
    PhysicsDoubleBuffer& getDoubleBuffer() { return db; }

    void togglePause() { paused.store(!paused.load()); }
    void setPause(bool p) { paused.store(p); }
    bool isPaused() const { return paused.load(); }

    void stepOnce(f64 dT) { performStep(dT); }
private:
    void ensureBuffersCapacity(size_t sz);
    void performStep(f32 dT);
    
    void physicsLoop();
};

extern PhysicsSystem phySys;

#endif // PHYSICS_SYS_H