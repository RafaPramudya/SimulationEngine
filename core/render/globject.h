#ifndef GLOBJECT_H
#define GLOBJECT_H

#include "utils/types.h"

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