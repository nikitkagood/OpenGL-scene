#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <assimp/Importer.hpp>

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
    std::string type;
    aiString path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Mesh_Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Mesh_Texture> textures);
    Mesh(const Mesh& copy);
    Mesh(Mesh&& moved) noexcept;

    ~Mesh();

    void Draw_mesh(Shader& shader);

private:
    VertexArray VAO;
    VertexBuffer VBO = { vertices.data(), vertices.size() * sizeof(Vertex) };
    IndexBuffer IBO = { indices.data(), indices.size() };
    VertexBufferLayout vertex_layout{3.0f, 3.0f, 2.0f, 3.0f, 3.0f};

    void setupMesh();

};