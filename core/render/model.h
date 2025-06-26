#ifndef MODEL_H
#define MODEL_H

#include "object.h"
#include "texture.h"
#include "utils/types.h"

#include <optional>
#include <utility>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

typedef u32 Index;
typedef u32 ShaderProgID;

class Transform {
public:
    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scalar;

    Transform()
        : translation(0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scalar(1.0f)
    {}
    ~Transform() = default;

    void translate(const glm::vec3& delta) {
        translation += delta;
    }

    void rotate(float deg, const glm::vec3& axis) {
        glm::quat delta = glm::angleAxis(glm::radians(deg), glm::normalize(axis));
        rotation = delta * rotation;
    }

    void scale(const glm::vec3& factor) {
        scalar *= factor;
    }

    glm::mat4 getMatrix() {
        glm::mat4 model(1.0f);
        model = glm::translate(model, translation);
        model *= glm::mat4_cast(rotation);
        model = glm::scale(model, scalar);
        return model;
    }
};


class Mesh {
private:
    std::optional<Object> GLObj;

    void setupMesh();
public:
    Mesh(std::vector<Vertex> vertices, std::vector<Index> indices, std::vector<Texture> textures);
    Mesh(f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz);
    ~Mesh() = default;

    void draw(ShaderProgID pId);

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

    void draw(ShaderProgID pId) {
        for (auto& mesh : meshes)
            mesh.draw(pId);
    }

    Transform transform;
private:
    std::vector<Mesh> meshes;
    std::string directory;

    std::vector<Texture> loaded;

    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType typeName);
};

#endif