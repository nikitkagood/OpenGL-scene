#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<array>
#include<cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL.h> 

#include"Mesh.h"
#include"Material.h"

#include"Profiler.h"

//Model requirements: each (assimp) Mesh MUST have at least Positions

//TODO: models multiple texture coordinates support

class IModel
{
protected:
    IModel() = default;

    IModel(const IModel& copy) = delete;

    virtual void Draw(Shader& shader)
    {
        assert(false); //IModel::Draw is not meant to be called
    }
private:

};

class SimpleModel : public IModel //code-described model as opposed to file-based model
{
public:
    SimpleModel()
    {
        setup();
    }

    void Draw(Shader& shader) override
    {
        shader.Bind();
        vao.Bind();

        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36)); 
    }

    //void SetPosition(const glm::vec3& new_position)
    //{
    //    position = new_position;
    //}

    //void SetPosition(glm::vec3&& new_position)
    //{
    //    position = new_position;
    //}

    glm::vec3 GetPosition()
    {
        return position;
    }

    const glm::vec3 GetPosition() const
    {
        return position;
    }

    glm::vec3 position;
private:
    void setup() 
    {
        vbo.Bind();
        vbo.SetBufferData();

        vao.AddBuffer(vbo, layout);

        vbo.Unbind();
        vao.Unbind();
    }

    unsigned int textureFromFile(const std::string& path)
    {
        unsigned int id = SOIL_load_OGL_texture(path.c_str(), 0, 0, SOIL_FLAG_INVERT_Y);

        if (!id)
        {
            std::cout << "ERROR::SOIL::Texture failed to load at path: " << path << std::endl;
            std::cout << SOIL_last_result() << std::endl;
        }

        return id;
    }


    array<float, 288> vertices = {
        // positions          // normals                // texture coords
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
    };
    //Material material{ { 1.0f, 0.5f, 0.31f }, { 0.5f, 0.5f, 0.5f }, 32.0f };

    VertexArray vao;
    VertexBuffer vbo{ vertices.data(), sizeof(vertices) };

    VertexBufferLayout layout{ 3.0f, 3.0f, 2.0f };
    std::vector<Mesh_Texture> textures;
};

class Model : public IModel
{
public:
    Model(std::string path);

    //copy constructor; at this point it's better to delete it, plus underlying classes are not copyable anyway
    Model(const Model& copy) = delete;

    void Draw(Shader& shader) override;

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Mesh_Texture> textures_loaded;
    
    void loadModel(std::string path);

    void processNode(aiNode* node, const aiScene* scene); //recursive function; to establish relations (parent-child) between model parts (this is not done yet)

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Mesh_Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName); //texture paths are relative to model folder; absolute paths won't work

    unsigned int textureFromFile(const aiString& path, const std::string& directory);

    static void to_glm_vec3(glm::vec3& destination, aiVector3D& mVertices);
    static void to_glm_vec2(glm::vec2& destination, aiVector3D& mVertices);

};
