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
    unsigned int nr_diffuse = 0;
    unsigned int nr_specular = 0;
    unsigned int nr_normal = 0;
    unsigned int nr_height = 0;

    for (GLuint i = 0; i < textures.size(); i++)
    {
        std::string number;

        switch(textures[i].type)
        {
        case aiTextureType::aiTextureType_DIFFUSE:
            number = std::to_string(++nr_diffuse);
            break;
        case aiTextureType::aiTextureType_SPECULAR:
            number = std::to_string(++nr_specular);
            break;
        case aiTextureType::aiTextureType_NORMALS:
            continue;
            //disabled for now
            //number = std::to_string (++nr_normal);
            //break;
        case aiTextureType::aiTextureType_HEIGHT:
            continue; 
            //disabled for now
            //number = std::to_string(++nr_height);
            //break;

        default:
            throw std::runtime_error("Mesh::Unexpected texture type");
            break;
        }

        //TODO glBindTextureUnit(0, texture0); 
        glActiveTexture(GL_TEXTURE0 + i);
        shader.SetUniform1i("materials[" + number + "]." + texture_type_to_string.at(textures[i].type), i);
        GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
    }

    glActiveTexture(GL_TEXTURE0);

    //drawing meshes
    VAO.Bind();
    IBO.Bind();
    
    //how many materials slots we used
    unsigned int nr_materials = std::max({ nr_diffuse, nr_specular, nr_normal, nr_height });

    shader.SetUniform1ui("nr_materials", nr_materials);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    VAO.Unbind();
}

void Mesh::setupMesh()
{
    VAO.Bind();
    VAO.AddBuffer(VBO, vertex_layout);

    VBO.Bind();
    VBO.SetBufferData();

    VAO.Unbind();
}