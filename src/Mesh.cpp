#pragma once

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Mesh_Texture> textures) : vertices(vertices), indices(indices), textures(textures)
{
#ifdef DEBUG_MODE_ON
    std::cerr << "Mesh constructor" << std::endl;
#endif // DEBUG_MODE_ON

    setupMesh();
}

#ifdef COPY_CONSTRUCTORS_ON
Mesh::Mesh(const Mesh& copy) : vertices(copy.vertices), indices(copy.indices), textures(copy.textures) //, VAO(copy.VAO), VBO(copy.VBO), IBO(copy.IBO), vertex_layout(copy.vertex_layout)
{
#ifdef DEBUG_MODE_ON
    std::cerr << "Mesh copy constructor" << std::endl;
#endif // DEBUG_MODE_ON

}
#else
Mesh::Mesh(const Mesh& copy)
{
    assert(false);
};
#endif // COPY_CONSTRUCTORS_ON


//move constructor
Mesh::Mesh(Mesh&& moved) noexcept : VAO(std::move(moved.VAO)), VBO(std::move(moved.VBO)), IBO(std::move(moved.IBO)), vertex_layout(std::move(moved.vertex_layout)), vertices(std::move(moved.vertices)), indices(std::move(moved.indices)), textures(std::move(moved.textures))
{
#ifdef DEBUG_MODE_ON
    std::cerr << "Mesh move constructor" << std::endl;
#endif // DEBUG_MODE_ON

}

Mesh::~Mesh()
{
#ifdef DEBUG_MODE_ON
    std::cerr << "Mesh destructor" << std::endl;
#endif // DEBUG_MODE_ON
}

void Mesh::Draw_mesh(Shader& shader)
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

void Mesh::setupMesh()
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