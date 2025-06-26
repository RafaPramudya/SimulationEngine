#include "model.h"

#include "glad/glad.h"

#include <cstdio>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Index> indices, std::vector<Texture> textures) {
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    setupMesh();
}
Mesh::Mesh(f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz) {
    auto reserveSize = vertsSz / (8 * sizeof(float));
    vertices.reserve(reserveSize);

    u32 maxLoop = (vertsSz / sizeof(float));
    for (u32 i = 0; i < maxLoop; i += 8) {
        Vertex vert;
        {
            glm::vec3 val;
            val.x = lpverts[i];
            val.y = lpverts[i + 1];
            val.z = lpverts[i + 2];
            vert.Position = val;
        }
        {
            glm::vec3 val;
            val.x = lpverts[i + 3];
            val.y = lpverts[i + 4];
            val.z = lpverts[i + 5];
            vert.Normal = val;
        }
        {
            glm::vec2 val;
            val.x = lpverts[i + 6];
            val.y = lpverts[i + 7];
            vert.TexCoords = val;
        }
        vertices.push_back(vert);
    }

    maxLoop = indsSz / sizeof(Index);
    indices.reserve(maxLoop);
    for (u32 i = 0; i < maxLoop; i++) {
        indices.push_back(lpinds[i]);
    }

    setupMesh();
}

void Mesh::setupMesh() {
    GLObj.emplace((f32*)&vertices[0], vertices.size() * sizeof(Vertex), &indices[0], indices.size() * sizeof(Index));
    GLObj->addAttribute(3);
    GLObj->addAttribute(3);
    GLObj->addAttribute(2);
    GLObj->compileAttribute();
    glBindVertexArray(0);
}

void Mesh::draw(ShaderProgID pID) {
    glUseProgram(pID);

    u32 diffuseNr = 0;
    for (u32 i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name;
        switch (textures[i].type)
        {
        case DIFFUSE:
            name = "diffuse" + std::to_string(diffuseNr++);
            break;
        default:
            name = "invalid";
            break;
        }

        glUniform1i(glGetUniformLocation(pID, name.c_str()), i);
        textures[i].bind();
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(GLObj->getVAO());
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Model::Model(std::string path) {
#ifdef DEBUG
    printf("Loading model : %s\n", path.c_str());
#endif
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("Error loading model : %s\n", import.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

Model::Model(f32* lpverts, size_t vertsSz, u32* lpinds, size_t indsSz) {
#ifdef DEBUG
    printf("Loading model manual\n");
#endif
    meshes.emplace_back(lpverts, vertsSz, lpinds, indsSz);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (u32 i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (u32 i = 0; i < node->mNumChildren; i++) 
        processNode(node->mChildren[i], scene);
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    std::vector<Texture> textures;

    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    for(u32 i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.Position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z   
        );

        vertex.Normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z   
        );

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            vertex.TexCoords = glm::vec2(0.0);
        }

        vertices.push_back(vertex);
    }

    for (u32 i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (u32 j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseTex = loadTextures(mat, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseTex.begin(), diffuseTex.end());
    }

    meshes.emplace_back(vertices, indices, textures);
}

std::vector<Texture> Model::loadTextures(aiMaterial* mat, aiTextureType typeName) {
    std::vector<Texture> textures;
    for (u32 i = 0; i < mat->GetTextureCount(typeName); i++) {
        aiString str;
        TextureType type;
        mat->GetTexture(typeName, i, &str);
        std::string texturePath = directory + '/' + str.C_Str();
        bool skip = false;
        for (const auto& tex : loaded) {
            if (tex.path == texturePath) {
                #ifdef DEBUG
                printf("Loading texture dari loaded\n");
                #endif
                textures.push_back(tex);
                skip = true;
                break;
            }
        }

        if (!skip){
            switch (typeName)
            {
            case aiTextureType_DIFFUSE:
                #ifdef DEBUG
                    printf("Load Diffuse Texture ");
                #endif
                type = TextureType::DIFFUSE;
                break;
            
            default:
                #ifdef DEBUG
                    printf("Invalid Texture\n");
                #endif
                type = TextureType::INVALID;
                break;
            }

            textures.emplace_back(texturePath, type, false);
            loaded.emplace_back(texturePath, type, false);
            // Debug print for texture path and OpenGL ID
            // printf("Loaded texture: %s\n", texturePath.c_str());
            // printf("Texture OpenGL ID: %u\n", loaded.back().getId());
        }
    }

    return textures;
}