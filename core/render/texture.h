#ifndef TEXTURES_H
#define TEXTURES_H

#include "utils/types.h"

#include <string>

enum TextureType {
    DIFFUSE,
    INVALID
};

class Texture {
public:
    Texture() = default;
    Texture(std::string imageName, TextureType type, bool flip = true);
    ~Texture();

    void bind();
    inline u32 getId() const { return id; }

    TextureType type;
    std::string path;
private:
    u32 id;
};

#endif