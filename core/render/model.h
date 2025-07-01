#ifndef MODEL_H
#define MODEL_H

#include "object.h"
#include "texture.h"
#include "shader.h"
#include "utils/types.h"

#include <optional>
#include <utility>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

typedef u32 Index;
// typedef u32 ShaderProgID;

class Mesh {
private:
    std::optional<Object> GLObj;

    void setupMesh();
public:
    Mesh(std::vector<Vertex> vertices, std::vector<Index> indices, std::vector<Texture> textures);
    Mesh(f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz);
    ~Mesh() = default;

    void draw(ShaderProg& shader);

    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    std::vector<Texture> textures;
};

class Model {
public:
    Model(std::string path);
    Model(f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz);
    ~Model() = default;

    template <typename... TArgs>
    void addTextureFirstMesh(TArgs&&... texArguments) {
        meshes[0].textures.emplace_back(std::forward<TArgs>(texArguments)...);
    }

    void draw(ShaderProg& shader) {
        for (auto& mesh : meshes)
            mesh.draw(shader);
    }
private:
    std::vector<Mesh> meshes;
    std::string directory;

    std::vector<Texture> loaded;

    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType typeName);
};

#endif