#include "texture.h"

#include "glad/glad.h"
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture createTexture(const char* imageName) {
    stbi_set_flip_vertically_on_load(true);

    
    i32 width, height, nrChannels;
    unsigned char* data = stbi_load(imageName, &width, &height, &nrChannels, 0);
    
    #ifdef DEBUG
    printf("Loading gambar : %s, width : %d, height : %d, nrChannels : %d\n", imageName,width, height, nrChannels);
    #endif
    assert(data);

    u32 texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Swap BGR to RGB if needed (common in some JPEGs)
    // if (nrChannels == 3) {
    //     for (int i = 0; i < width * height; i++) {
    //         unsigned char tmp = data[i * 3];
    //         data[i * 3] = data[i * 3 + 2];
    //         data[i * 3 + 2] = tmp;
    //     }
    // }

// Upload to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, (nrChannels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    Texture tex = {.id=texId};
    return tex;
}

void freeTexture(Texture tex) {
    glDeleteTextures(1, &tex.id);
}