#include "texture.h"

#include "glad/glad.h"
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture createTexture(const char* imageName) {
    stbi_set_flip_vertically_on_load(true);

    #ifdef DEBUG
    printf("Loading gambar : %s\n", imageName);
    #endif

    i32 width, height, nrChannels;
    unsigned char* data = stbi_load(imageName, &width, &height, &nrChannels, 0);

    assert(data);

    u32 texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum format = GL_RGB;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    Texture tex = {.id=texId};
    return tex;
}

void freeTexture(Texture tex) {
    glDeleteTextures(1, &tex.id);
}