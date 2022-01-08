#pragma once

#include<iostream>
#include<vector>
#include<list>
#include<string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL.h> 
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include"Mesh.h"

#include "Profiler.h"

//Model requirements: each (assimp) Mesh MUST have at least Positions

//TODO: models multiple texture coordinates support

class Model
{
public:
    Model(std::string path)
    {
#ifdef BENCHMARK_MODE_ON
        LOG_DURATION("BENCHMARK: model loading");
#endif // BENCHMARK_MODE_ON
        loadModel(path);
    }

    //copy constructor; at this point it's better to delete it, plus underlying classes are not copyable anyway
    Model(const Model& copy) = delete;

    void Draw(Shader& shader)
    {
        for (auto& i : meshes)
        {
            i.Draw_mesh(shader);
        }
    }
private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Mesh_Texture> textures_loaded;
    
    void loadModel(std::string path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR: Could not load a model. ASSIMP: " << importer.GetErrorString() << std::endl;
#ifdef EXCEPTION_MODEL_FAILED_TO_LOAD
            throw std::runtime_error("Could not load a model");
#endif // EXCEPTION_MODEL_FAILED_TO_LOAD
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene) //recursive function; to establish relations (parent-child) between model parts (this is not done yet)
    {
        //обработать все полигональные сетки в узле(если есть)
        meshes.reserve(node->mNumMeshes);
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* assimp_mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.emplace_back(std::move(processMesh(assimp_mesh, scene))); //Mesh and underlying classes call destructor here
        } 
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Mesh_Texture> textures;

        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumFaces);
        textures.reserve(3); //there is usually at least 3 textures - diffuse, specular, normal
        

        //processing of coordinates, normals and texture coords
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vec3_glm(0.0f); // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vec3_glm.x = mesh->mVertices[i].x;
            vec3_glm.y = mesh->mVertices[i].y;
            vec3_glm.z = mesh->mVertices[i].z;

            vertex.Position = vec3_glm;
            // normals
            if (mesh->HasNormals())
            {
                vec3_glm.x = mesh->mNormals[i].x;
                vec3_glm.y = mesh->mNormals[i].y;
                vec3_glm.z = mesh->mNormals[i].z;
                vertex.Normal = vec3_glm;
            }

            // texture coordinates
            glm::vec2 vec2_glm(0.0f);
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec2_glm.x = mesh->mTextureCoords[0][i].x;
                vec2_glm.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec2_glm;


                //if (mesh->HasTangentsAndBitangents()) //if aiProcess_CalcTangentSpace is set, this will always be true
                //{
                    //tangent
                    vec3_glm.x = mesh->mTangents[i].x;
                    vec3_glm.y = mesh->mTangents[i].y;
                    vec3_glm.z = mesh->mTangents[i].z;
                    vertex.Tangent = vec3_glm;

                    //bitangent
                    vec3_glm.x = mesh->mBitangents[i].x;
                    vec3_glm.y = mesh->mBitangents[i].y;
                    vec3_glm.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vec3_glm;
                //}
            }
            else vertex.TexCoords = vec2_glm;

            vertices.push_back(std::move(vertex));
        }
        //indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace& face = mesh->mFaces[i];
            for (unsigned j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        //material
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<Mesh_Texture> diffuseMaps = std::move(loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse"));
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Mesh_Texture> specularMaps = std::move(loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular"));
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            std::vector<Mesh_Texture> normalMaps = std::move(loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal"));
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

            std::vector<Mesh_Texture> heightMaps = std::move(loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height"));
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        }

        return { vertices, indices, textures };
    }

    std::vector<Mesh_Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) //texture paths are relative to model folder; absolute paths won't work
    {
        std::vector<Mesh_Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                //if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
                if (textures_loaded[j].path == str)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) //if texture haven't loaded
            {
                Mesh_Texture texture;
                texture.id = TextureFromFile(str, directory);
                texture.type = typeName;
                texture.path = str;
                textures.push_back(texture);

                textures_loaded.push_back(std::move(texture)); //add texture to list of already loaded  textures
            }
        }
        return textures;
    }

    unsigned int TextureFromFile(const aiString& path, const std::string& directory)
    {
        std::string filename = directory + '/' + path.C_Str();

        unsigned int id = SOIL_load_OGL_texture(filename.c_str(), 0, 0, SOIL_FLAG_INVERT_Y);

        if (!id)
        {
            std::cout << "ERROR::SOIL::Texture failed to load at path: " << filename << std::endl;
            std::cout << SOIL_last_result() << std::endl;
        }

        return id;
    }

};
