#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>

#include <string>
#include <vector>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Mesh_Texture {
    unsigned int id;
    aiTextureType type;
    aiString path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Mesh_Texture> textures; //this vector contains only data ABOUT textures, not textures themselves

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Mesh_Texture> textures);
    Mesh(const Mesh& copy);
    Mesh(Mesh&& moved) noexcept; //noexcept is mandatory, otherwise C26439 and the code will try to call copy constructor

    ~Mesh();

    void Draw_mesh(Shader& shader);

private:
    VertexArray VAO;
    VertexBuffer VBO = { vertices.data(), vertices.size() * sizeof(Vertex) };
    IndexBuffer IBO = { indices.data(), indices.size() };
    VertexBufferLayout vertex_layout{3.0f, 3.0f, 2.0f, 3.0f, 3.0f};

    void setupMesh();

    std::map<aiTextureType, std::string> texture_type_to_string
    {
        { aiTextureType::aiTextureType_NONE, "none" },
        { aiTextureType::aiTextureType_DIFFUSE, "diffuse" },
        { aiTextureType::aiTextureType_SPECULAR, "specular" },
        { aiTextureType::aiTextureType_AMBIENT, "ambient" },
        { aiTextureType::aiTextureType_EMISSIVE, "emissive" },
        { aiTextureType::aiTextureType_HEIGHT, "height" },
        { aiTextureType::aiTextureType_NORMALS, "normals" },
        { aiTextureType::aiTextureType_SHININESS, "shininess" },
        { aiTextureType::aiTextureType_OPACITY, "opacity" },
        { aiTextureType::aiTextureType_DISPLACEMENT, "displacement" },
        { aiTextureType::aiTextureType_LIGHTMAP, "lightmap" },
        { aiTextureType::aiTextureType_REFLECTION, "reflection" },
        { aiTextureType::aiTextureType_BASE_COLOR, "base_color" },
        { aiTextureType::aiTextureType_NORMAL_CAMERA, "normal_camera" },
        { aiTextureType::aiTextureType_EMISSION_COLOR, "emission_color" },
        { aiTextureType::aiTextureType_METALNESS, "metalness" },
        { aiTextureType::aiTextureType_DIFFUSE_ROUGHNESS, "roughness" },
        { aiTextureType::aiTextureType_AMBIENT_OCCLUSION, "ambient_occlusion" },
        { aiTextureType::aiTextureType_UNKNOWN, "unknown" },
    };
};


