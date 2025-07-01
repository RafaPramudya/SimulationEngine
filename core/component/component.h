#ifndef COMPONENT_H
#define COMPONENT_H

#include <array>
#include <bitset>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>

#include "utils/types.h"

static constexpr auto maxComponents = 32;

class Component;
class Entity;
class EntityManager;

template <typename T>
using ComponentPool = std::vector<T>;
using ComponentArr = std::array<Component*, maxComponents>;
using ComponentBitset = std::bitset<maxComponents>;

static inline u32 getComponentTypeID() {
    static u32 id = 0;
    return id++;
}

template <typename T>
static inline u32 getComponentID() {
    static auto typeId = getComponentTypeID();
    if (typeId >= maxComponents) throw std::exception("ID lebih dari maxComponent");
    return typeId;
}

class Component {
public:
    // static void* addMembersToPool() = 0;
    virtual void update() {}
    virtual void render() {}

    Entity* entity;
};

class Entity {
private:
    ComponentBitset cBitset;
    ComponentArr cArray;
    std::vector<std::unique_ptr<Component>> cArrPtr;

    bool active = true;
public:
    Entity() { cBitset.reset(); }
    ~Entity() = default;

    // Prevent copying
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    // Allow moving
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;

    bool isActive() const { return active; }
    void destroy() { active = false; }

    template <typename T, typename... TArgs> 
    T& addComponent(TArgs&&... args) {
        // auto ptr = std::make_unique<T>(std::forward<TArgs>(args)...);
        // ptr->entity = this;
        // auto cId = getComponentID<T>();
        // cBitset[cId] = true;
        // cArray[cId] = ptr.get();
        // cArrPtr.emplace_back(std::move(ptr));
        // return *static_cast<T*>(cArray[cId]);

        T* ptr(new T(std::forward<TArgs>(args)...));
        ptr->entity = this;
        std::unique_ptr<Component> u_ptr{ptr};
        cArrPtr.emplace_back(std::move(u_ptr));
        auto cId = getComponentID<T>();
        cBitset[cId] = true;
        cArray[cId] = ptr;
        return *ptr;
    }
    template <typename T>
    T& getComponent() {
        auto ptr = cArray[getComponentID<T>()];
        return *static_cast<T*>(ptr);
    }
    template <typename T>
    bool hasComponent() const {
        return cBitset[getComponentID<T>()];
    }

    void update() {
        for (auto& c : cArrPtr) c->update();
    }
    void render() {
        for (auto& c : cArrPtr) c->render();
    }
};

class EntityManager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
public:
    Entity& addEntity() {
        Entity* e(new Entity());
        std::unique_ptr<Entity> u_ptr{e};
        entities.emplace_back(std::move(u_ptr));
        return *e;
    }

    void update() {
        for (auto& e : entities) e->update();
    }
    void render() {
        for (auto& e : entities) e->render();
    }
    void refresh() {
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
            [](const std::unique_ptr<Entity>& e) { return !e->isActive(); }),
            entities.end()
        );
    }       
};

extern EntityManager eManager;

#endif // COMPONENT_H