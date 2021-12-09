#pragma once

#include "SimpleModel.h"

void SimpleModel::Draw(Shader& shader)
{
    shader.Bind();
    vao.Bind();

    for(auto& i : textures)
    {
        switch(i.GetType())
        {
        case(sm_TextureType::DIFFUSE):
            textures[0].Use(0, shader.GetID(), "material.diffuse");
            break;
        case(sm_TextureType::SPECULAR):
            textures[1].Use(1, shader.GetID(), "material.specular");
            break;
        default:
            break;
        }
    }

    if(!materials.empty())
    {
        materials[0].Use(shader);
    }
    //glActiveTexture(GL_TEXTURE0);

    ////drawing meshes
    //VAO.Bind();
    //IBO.Bind();

    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    //VAO.Unbind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36)); 
}


void SimpleModel::addTexture(const SimpleTexture& texture)
{
    textures.push_back(texture);
}

void SimpleModel::addTexture(SimpleTexture&& texture)
{
    textures.push_back(texture);
}

void SimpleModel::addMaterial(const Material& material)
{
    materials.push_back(material);
}

void SimpleModel::addMaterial(Material&& material)
{
    materials.push_back(material);
}


void SimpleModel::setup()
{
    vbo.Bind();
    vbo.SetBufferData();

    vao.AddBuffer(vbo, layout);

    vbo.Unbind();
    vao.Unbind();
}

//hardcoded cube vertices
//std::array<float, 288> vertices = {
//    // positions          // normals                // texture coords
//    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
//    0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
//    0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
//    0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
//    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
//    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
//
//    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
//    0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
//    0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
//    0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
//    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
//    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
//
//    -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
//    -0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
//    -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
//    -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
//    -0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
//    -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
//
//    0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
//    0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
//    0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
//    0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
//    0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
//    0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
//
//    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
//    0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
//    0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
//    0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
//    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
//    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
//
//    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
//    0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
//    0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
//    0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
//    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
//    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
//};
