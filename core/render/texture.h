#ifndef TEXTURES_H
#define TEXTURES_H

#include "utils/types.h"

class Texture {
public:
    Texture() = default;
    Texture(const char* imageName);
    ~Texture();

    void bind();
    inline u32 getId() const { return id; }
private:
    u32 id;
};

#endif