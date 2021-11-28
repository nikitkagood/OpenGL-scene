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

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Mesh_Texture> textures) : vertices(vertices), indices(indices), textures(textures)
    {
#ifdef DEBUG_MODE_ON
        std::cerr << "Mesh constructor" << std::endl;
#endif // DEBUG_MODE_ON

        setupMesh();
    }

#ifdef COPY_CONSTRUCTORS_ON
    Mesh(const Mesh& copy) : vertices(copy.vertices), indices(copy.indices), textures(copy.textures) //, VAO(copy.VAO), VBO(copy.VBO), IBO(copy.IBO), vertex_layout(copy.vertex_layout)
    {
#ifdef DEBUG_MODE_ON
        std::cerr << "Mesh copy constructor" << std::endl;
#endif // DEBUG_MODE_ON

    }
#else
    //copy constructor; at this point it's better to delete it
    Mesh(const Mesh& copy) = delete;
#endif // COPY_CONSTRUCTORS_ON


    //move constructor
    Mesh(Mesh&& moved) noexcept : VAO(std::move(moved.VAO)), VBO(std::move(moved.VBO)), IBO(std::move(moved.IBO)), vertex_layout(std::move(moved.vertex_layout)), vertices(std::move(moved.vertices)), indices(std::move(moved.indices)), textures(std::move(moved.textures))
    {
#ifdef DEBUG_MODE_ON
        std::cerr << "Mesh move constructor" << std::endl;
#endif // DEBUG_MODE_ON

    }

    ~Mesh()
    {
#ifdef DEBUG_MODE_ON
        std::cerr << "Mesh destructor" << std::endl;
#endif // DEBUG_MODE_ON
    }

    void Draw_mesh(Shader& shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string name = textures[i].type;
            std::string number;

            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to stream

            shader.SetUniform1i((name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glActiveTexture(GL_TEXTURE0);

        //drawing meshes
        VAO.Bind();
        IBO.Bind();

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        VAO.Unbind();
    }

private:
    VertexArray VAO;
    VertexBuffer VBO = { vertices.data(), vertices.size() * sizeof(Vertex) };
    IndexBuffer IBO = { indices.data(), indices.size() };
    VertexBufferLayout vertex_layout{3.0f, 3.0f, 2.0f, 3.0f, 3.0f};

    void setupMesh()
    {
        //old way of creating a layout
        //vertex_layout.Push<float>(3);
        //vertex_layout.Push<float>(3);
        //vertex_layout.Push<float>(2);
        //vertex_layout.Push<float>(3);
        //vertex_layout.Push<float>(3);

        VAO.Bind();
        VAO.AddBuffer(VBO, vertex_layout);

        VBO.Bind();
        VBO.SetBufferData();

        VAO.Unbind();
    }

};