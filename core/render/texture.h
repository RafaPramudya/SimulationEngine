#ifndef TEXTURES_H
#define TEXTURES_H

#include "utils/types.h"

typedef struct Texture_s {
    u32 id;
} Texture;

Texture createTexture(const char* imageName);
void freeTexture(Texture tex);

#endif